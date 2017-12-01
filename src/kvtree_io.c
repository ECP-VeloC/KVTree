/*
 * Copyright (c) 2009, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by Adam Moody <moody20@llnl.gov>.
 * LLNL-CODE-411039.
 * All rights reserved.
 * This file was originally part of The Scalable Checkpoint / Restart (SCR) library.
 * For details, see https://sourceforge.net/projects/scalablecr/
 * Please also read this file: LICENSE.TXT.
*/

/* Implements a reliable open/read/write/close interface via open and close.
 * Implements directory manipulation functions. */

/* Please note todos in the cppr section; an optimization of using CPPR apis is
 * planned for upcoming work */

#include "kvtree.h"
#include "kvtree_err.h"
#include "kvtree_io.h"
#include "kvtree_helpers.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

/* variable length args */
#include <stdarg.h>
#include <errno.h>

/* basename/dirname */
#include <unistd.h>
#include <libgen.h>

/* compute crc32 */
#include <zlib.h>

/* flock */
#include <sys/file.h>

/* gettimeofday */
#include <sys/time.h>

/*
=========================================
open/lock/close/read/write functions
=========================================
*/

/* returns user's current mode as determine by his umask */
mode_t kvtree_getmode(int read, int write, int execute)
{
  /* lookup current mask and set it back */
  mode_t old_mask = umask(S_IWGRP | S_IWOTH);
  umask(old_mask);

  mode_t bits = 0;
  if (read) {
    bits |= (S_IRUSR | S_IRGRP | S_IROTH);
  }
  if (write) {
    bits |= (S_IWUSR | S_IWGRP | S_IWOTH);
  }
  if (execute) {
    bits |= (S_IXUSR | S_IXGRP | S_IXOTH);
  }

  /* convert mask to mode */
  mode_t mode = bits & ~old_mask & 0777;
  return mode;
}

/* open file with specified flags and mode, retry open a few times on failure */
int kvtree_open(const char* file, int flags, ...)
{
  /* extract the mode (see man 2 open) */
  int mode_set = 0;
  mode_t mode = 0;
  if (flags & O_CREAT) {
    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);
    mode_set = 1;
  }

  int fd = -1;
  if (mode_set) {
    fd = open(file, flags, mode);
  } else {
    fd = open(file, flags);
  }
  if (fd < 0) {
    kvtree_dbg(1, "Opening file: open(%s) errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );

    /* try again */
    int tries = KVTREE_OPEN_TRIES;
    while (tries && fd < 0) {
      usleep(KVTREE_OPEN_USLEEP);
      if (mode_set) {
        fd = open(file, flags, mode);
      } else {
        fd = open(file, flags);
      }
      tries--;
    }

    /* if we still don't have a valid file, consider it an error */
    if (fd < 0) {
      kvtree_err("Opening file: open(%s) errno=%d %s @ %s:%d",
        file, errno, strerror(errno), __FILE__, __LINE__
      );
    }
  }
  return fd;
}

/* fsync and close file */
int kvtree_close(const char* file, int fd)
{
  /* fsync first */
  if (fsync(fd) < 0) {
    /* print warning that fsync failed */
    kvtree_dbg(2, "Failed to fsync file descriptor: %s errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
  }

  /* now close the file */
  if (close(fd) != 0) {
    /* hit an error, print message */
    kvtree_err("Closing file descriptor %d for file %s: errno=%d %s @ %s:%d",
      fd, file, errno, strerror(errno), __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }

  return KVTREE_SUCCESS;
}

int kvtree_file_lock_read(const char* file, int fd)
{
  #ifdef KVTREE_FILE_LOCK_USE_FLOCK
    if (flock(fd, LOCK_SH) != 0) {
      kvtree_err("Failed to acquire file lock on %s: flock(%d, %d) errno=%d %s @ %s:%d",
        file, fd, LOCK_SH, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  #ifdef KVTREE_FILE_LOCK_USE_FCNTL
    struct flock lck;
    lck.l_type = F_RDLCK;
    lck.l_whence = 0;
    lck.l_start = 0L;
    lck.l_len = 0L; //locking the entire file

    if(fcntl(fd, F_SETLK, &lck) < 0) {
      kvtree_err("Failed to acquire file read lock on %s: fnctl(%d, %d) errno=%d %s @ %s:%d",
        file, fd, F_RDLCK, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  return KVTREE_SUCCESS;
}

int kvtree_file_lock_write(const char* file, int fd)
{
  #ifdef KVTREE_FILE_LOCK_USE_FLOCK
    if (flock(fd, LOCK_EX) != 0) {
      kvtree_err("Failed to acquire file lock on %s: flock(%d, %d) errno=%d %s @ %s:%d",
        file, fd, LOCK_EX, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  #ifdef KVTREE_FILE_LOCK_USE_FCNTL
    struct flock lck;
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = 0L;
    lck.l_len = 0L; //locking the entire file

    if(fcntl(fd, F_SETLK, &lck) < 0) {
      kvtree_err("Failed to acquire file read lock on %s: fnctl(%d, %d) errno=%d %s @ %s:%d",
        file, fd, F_WRLCK, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  return KVTREE_SUCCESS;
}

int kvtree_file_unlock(const char* file, int fd)
{
  #ifdef KVTREE_FILE_LOCK_USE_FLOCK
    if (flock(fd, LOCK_UN) != 0) {
      kvtree_err("Failed to acquire file lock on %s: flock(%d, %d) errno=%d %s @ %s:%d",
        file, fd, LOCK_UN, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  #ifdef KVTREE_FILE_LOCK_USE_FCNTL
    struct flock lck;
    lck.l_type = F_UNLCK;
    lck.l_whence = 0;
    lck.l_start = 0L;
    lck.l_len = 0L; //locking the entire file

    if(fcntl(fd, F_SETLK, &lck) < 0) {
      kvtree_err("Failed to acquire file read lock on %s: fnctl(%d, %d) errno=%d %s @ %s:%d",
        file, fd, F_UNLCK, errno, strerror(errno), __FILE__, __LINE__
      );
      return KVTREE_FAILURE;
    }
  #endif

  return KVTREE_SUCCESS;
}

/* opens specified file and waits on a lock before returning the file descriptor */
int kvtree_open_with_lock(const char* file, int flags, mode_t mode)
{
  /* open the file */
  int fd = kvtree_open(file, flags, mode);
  if (fd < 0) {
    kvtree_err("Opening file for write: kvtree_open(%s) errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    return fd;
  }

  /* acquire an exclusive file lock */
  int ret = kvtree_file_lock_write(file, fd);
  if (ret != KVTREE_SUCCESS) {
    close(fd);
    return ret;
  }

  /* return the opened file descriptor */
  return fd;
}

/* unlocks the specified file descriptor and then closes the file */
int kvtree_close_with_unlock(const char* file, int fd)
{
  /* release the file lock */
  int ret = kvtree_file_unlock(file, fd);
  if (ret != KVTREE_SUCCESS) {
    return ret;
  }

  /* close the file */
  return kvtree_close(file, fd);
}

/* seek file descriptor to specified position */
int kvtree_lseek(const char* file, int fd, off_t pos, int whence)
{
  off_t rc = lseek(fd, pos, whence);
  if (rc == (off_t)-1) {
    kvtree_err("Error seeking %s: errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* reliable read from file descriptor (retries, if necessary, until hard error) */
ssize_t kvtree_read(const char* file, int fd, void* buf, size_t size)
{
  ssize_t n = 0;
  int retries = 10;
  while (n < size)
  {
    int rc = read(fd, (char*) buf + n, size - n);
    if (rc  > 0) {
      n += rc;
    } else if (rc == 0) {
      /* EOF */
      return n;
    } else { /* (rc < 0) */
      /* got an error, check whether it was serious */
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }

      /* something worth printing an error about */
      retries--;
      if (retries) {
        /* print an error and try again */
        kvtree_err("Error reading %s: read(%d, %x, %ld) errno=%d %s @ %s:%d",
          file, fd, (char*) buf + n, size - n, errno, strerror(errno), __FILE__, __LINE__
        );
      } else {
        /* too many failed retries, give up */
        kvtree_err("Giving up read of %s: read(%d, %x, %ld) errno=%d %s @ %s:%d",
	  file, fd, (char*) buf + n, size - n, errno, strerror(errno), __FILE__, __LINE__
        );
        exit(1);
      }
    }
  }
  return n;
}

/* reliable write to opened file descriptor (retries, if necessary, until hard error) */
ssize_t kvtree_write(const char* file, int fd, const void* buf, size_t size)
{
  ssize_t n = 0;
  int retries = 10;
  while (n < size)
  {
    ssize_t rc = write(fd, (char*) buf + n, size - n);
    if (rc > 0) {
      n += rc;
    } else if (rc == 0) {
      /* something bad happened, print an error and abort */
      kvtree_err("Error writing %s: write(%d, %x, %ld) returned 0 @ %s:%d",
	file, fd, (char*) buf + n, size - n, __FILE__, __LINE__
      );
      exit(1);
    } else { /* (rc < 0) */
      /* got an error, check whether it was serious */
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }

      /* something worth printing an error about */
      retries--;
      if (retries) {
        /* print an error and try again */
        kvtree_err("Error writing %s: write(%d, %x, %ld) errno=%d %s @ %s:%d",
          file, fd, (char*) buf + n, size - n, errno, strerror(errno), __FILE__, __LINE__
        );
      } else {
        /* too many failed retries, give up */
        kvtree_err("Giving up write to %s: write(%d, %x, %ld) errno=%d %s @ %s:%d",
          file, fd, (char*) buf + n, size - n, errno, strerror(errno), __FILE__, __LINE__
        );
        exit(1);
      }
    }
  }
  return n;
}

/* make a good attempt to read from file (retries, if necessary, return error if fail) */
ssize_t kvtree_read_attempt(const char* file, int fd, void* buf, size_t size)
{
  ssize_t n = 0;
  int retries = 10;
  while (n < size)
  {
    int rc = read(fd, (char*) buf + n, size - n);
    if (rc  > 0) {
      n += rc;
    } else if (rc == 0) {
      /* EOF */
      return n;
    } else { /* (rc < 0) */
      /* got an error, check whether it was serious */
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }

      /* something worth printing an error about */
      retries--;
      if (retries) {
        /* print an error and try again */
        kvtree_err("Error reading file %s errno=%d %s @ %s:%d",
          file, errno, strerror(errno), __FILE__, __LINE__
        );
      } else {
        /* too many failed retries, give up */
        kvtree_err("Giving up read on file %s errno=%d %s @ %s:%d",
	  file, errno, strerror(errno), __FILE__, __LINE__
        );
        return -1;
      }
    }
  }
  return n;
}

/* make a good attempt to write to file (retries, if necessary, return error if fail) */
ssize_t kvtree_write_attempt(const char* file, int fd, const void* buf, size_t size)
{
  ssize_t n = 0;
  int retries = 10;
  while (n < size)
  {
    ssize_t rc = write(fd, (char*) buf + n, size - n);
    if (rc > 0) {
      n += rc;
    } else if (rc == 0) {
      /* something bad happened, print an error and abort */
      kvtree_err("Error writing file %s write returned 0 @ %s:%d",
        file, __FILE__, __LINE__
      );
      return -1;
    } else { /* (rc < 0) */
      /* got an error, check whether it was serious */
      if (errno == EINTR || errno == EAGAIN) {
        continue;
      }

      /* something worth printing an error about */
      retries--;
      if (retries) {
        /* print an error and try again */
        kvtree_err("Error writing file %s errno=%d %s @ %s:%d",
          file, errno, strerror(errno), __FILE__, __LINE__
        );
      } else {
        /* too many failed retries, give up */
        kvtree_err("Giving up write of file %s errno=%d %s @ %s:%d",
          file, errno, strerror(errno), __FILE__, __LINE__
        );
        return -1;
      }
    }
  }
  return n;
}

/* read line from file into buf with given size */
ssize_t kvtree_read_line(const char* file, int fd, char* buf, size_t size)
{
  /* read up to size-1 bytes from fd into buf until we find a newline or EOF */
  ssize_t n = 0;
  int found_end = 0;
  while (n < size-1 && !found_end) {
    /* read a character from the file */
    char c;
    ssize_t nread = kvtree_read(file, fd, &c, sizeof(c));

    if (nread > 0) {
      /* we read a character, copy it over to the buffer */
      buf[n] = c;
      n++;

      /* check whether we hit the end of the line */
      if (c == '\n') {
        found_end = 1;
      }
    } else if (nread == 0) {
      /* we hit the end of the file */
      found_end = 1;
    } else { /* nread < 0 */
      /* we hit an error */
      kvtree_err("Error reading from file %s @ %s:%d",
        file, __FILE__, __LINE__
      );
      return -1;
    }
  }

  /* tack on the NULL character */
  buf[n] = '\0';

  /* if we exit the while loop but didn't find the end of the line, the buffer was too small */
  if (!found_end) {
    kvtree_err("Buffer too small to read line from file %s @ %s:%d",
      file, __FILE__, __LINE__
    );
    return -1;
  }

  /* NOTE: we don't want to count the NULL which we added, but there is no need to adjust n here */
  return n;
}

/* write a formatted string to specified file descriptor */
ssize_t kvtree_writef(const char* file, int fd, const char* format, ...)
{
  /* write the formatted string to a buffer */
  char buf[KVTREE_MAX_LINE];
  va_list argp;
  va_start(argp, format);
  int n = vsnprintf(buf, sizeof(buf), format, argp);
  va_end(argp);

  /* check that our buffer was large enough */
  if (sizeof(buf) <= n) {
    /* TODO: instead of throwing a fatal error, we could allocate a bigger buffer and try again */

    kvtree_err("Buffer too small to hold formatted string for file %s @ %s:%d",
      file, __FILE__, __LINE__
    );
    exit(1);
  }

  /* write the string out to the file descriptor */
  ssize_t rc = kvtree_write(file, fd, buf, n);

  return rc;
}

/* logically concatenate n opened files and read count bytes from this logical file into buf starting
 * from offset, pad with zero on end if missing data */
int kvtree_read_pad_n(int n, char** files, int* fds,
                   char* buf, unsigned long count, unsigned long offset, unsigned long* filesizes)
{
  int i = 0;
  size_t pos = 0;
  off_t nseek = 0;
  off_t nread = 0;

  /* pass through files until we find the one containing our offset */
  while (i < n && (nseek + filesizes[i]) <= offset) {
    nseek += filesizes[i];
    i++;
  }

  /* seek to the proper position in the current file */
  if (i < n) {
      pos = offset - nseek;
      nseek += pos;
      if (lseek(fds[i], pos, SEEK_SET) == (off_t)-1) {
        /* our seek failed, return an error */
        kvtree_err("Failed to seek to byte %lu in %s @ %s:%d",
          pos, files[i], __FILE__, __LINE__
        );
        return KVTREE_FAILURE;
      }
  }

  /* read data from files */
  while (nread < count && i < n) {
    /* assume we'll read the remainder of the current file */
    size_t num_to_read = filesizes[i] - pos;

    /* if we don't need to read the whole remainder of the file, adjust to the smaller amount */
    if (num_to_read > count - nread) {
      num_to_read = count - nread;
    }

    /* read data from file and add to the total read count */
    if (kvtree_read_attempt(files[i], fds[i], buf + nread, num_to_read) != num_to_read) {
      /* our read failed, return an error */
      return KVTREE_FAILURE;
    }
    nread += num_to_read;

    /* advance to next file and seek to byte 0 */
    i++;
    if (i < n) {
      pos = 0;
      if (lseek(fds[i], pos, SEEK_SET) == (off_t)-1) {
        /* our seek failed, return an error */
        kvtree_err("Failed to seek to byte %lu in %s @ %s:%d",
          pos, files[i], __FILE__, __LINE__
        );
        return KVTREE_FAILURE;
      }
    }
  }

  /* if count is bigger than all of our file data, pad with zeros on the end */
  if (nread < count) {
    memset(buf + nread, 0, count - nread);
  }

  return KVTREE_SUCCESS;
}

/* write to an array of open files with known filesizes treating them as one single large file */
int kvtree_write_pad_n(int n, char** files, int* fds,
                    char* buf, unsigned long count, unsigned long offset, unsigned long* filesizes)
{
  int i = 0;
  size_t pos = 0;
  off_t nseek  = 0;
  off_t nwrite = 0;

  /* pass through files until we find the one containing our offset */
  while (i < n && (nseek + filesizes[i]) <= offset) {
    nseek += filesizes[i];
    i++;
  }

  /* seek to the proper position in the current file */
  if (i < n) {
      pos = offset - nseek;
      nseek += pos;
      if (lseek(fds[i], pos, SEEK_SET) == (off_t)-1) {
        /* our seek failed, return an error */
        kvtree_err("Failed to seek to byte %lu in %s @ %s:%d",
          pos, files[i], __FILE__, __LINE__
        );
        return KVTREE_FAILURE;
      }
  }

  /* write data to files */
  while (nwrite < count && i < n) {
    /* assume we'll write the remainder of the current file */
    size_t num_to_write = filesizes[i] - pos;

    /* if we don't need to write the whole remainder of the file, adjust to the smaller amount */
    if (num_to_write > count - nwrite) {
      num_to_write = count - nwrite;
    }

    /* write data to file and add to the total write count */
    if (kvtree_write_attempt(files[i], fds[i], buf + nwrite, num_to_write) != num_to_write) {
      /* our write failed, return an error */
      return KVTREE_FAILURE;
    }
    nwrite += num_to_write;

    /* advance to next file and seek to byte 0 */
    i++;
    if (i < n) {
      pos = 0;
      if (lseek(fds[i], pos, SEEK_SET) == (off_t)-1) {
        /* our seek failed, return an error */
        kvtree_err("Failed to seek to byte %lu in %s @ %s:%d",
          pos, files[i], __FILE__, __LINE__
        );
        return KVTREE_FAILURE;
      }
    }
  }

  /* if count is bigger than all of our file data, just throw the data away */
  if (nwrite < count) {
    /* NOTHING TO DO */
  }

  return KVTREE_SUCCESS;
}

/* given a filename, return number of bytes in file */
unsigned long kvtree_file_size(const char* file)
{
  /* get file size in bytes */
  unsigned long bytes = 0;
  struct stat stat_buf;
  int stat_rc = stat(file, &stat_buf);
  if (stat_rc == 0) {
    /*
    mode = stat_buf.st_mode;
    */
    bytes = stat_buf.st_size;
  }
  return bytes;
}

/* tests whether the file or directory exists */
int kvtree_file_exists(const char* file)
{
  /* check whether the file exists */
  if (access(file, F_OK) < 0) {
    /* TODO: would be nice to print a message here, but
     *       functions calling this expect it to be quiet
    kvtree_dbg(2, "File does not exist: %s errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    */
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* tests whether the file or directory is readable */
int kvtree_file_is_readable(const char* file)
{
  /* check whether the file can be read */
  if (access(file, R_OK) < 0) {
    /* TODO: would be nice to print a message here, but
     *       functions calling this expect it to be quiet
    kvtree_dbg(2, "File not readable: %s errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    */
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* tests whether the file or directory is writeable */
int kvtree_file_is_writeable(const char* file)
{
  /* check whether the file can be read */
  if (access(file, W_OK) < 0) {
    /* TODO: would be nice to print a message here, but
     *       functions calling this expect it to be quiet
    kvtree_dbg(2, "File not writeable: %s errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    */
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* delete a file */
int kvtree_file_unlink(const char* file)
{
  if (unlink(file) != 0) {
    kvtree_dbg(2, "Failed to delete file: %s errno=%d %s @ %s:%d",
      file, errno, strerror(errno), __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* opens, reads, and computes the crc32 value for the given filename */
int kvtree_crc32(const char* filename, uLong* crc)
{
  /* check that we got a variable to write our answer to */
  if (crc == NULL) {
    return KVTREE_FAILURE;
  }

  /* initialize our crc value */
  *crc = crc32(0L, Z_NULL, 0);

  /* open the file for reading */
  int fd = kvtree_open(filename, O_RDONLY);
  if (fd < 0) {
    kvtree_dbg(1, "Failed to open file to compute crc: %s errno=%d @ %s:%d",
      filename, errno, __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }

  /* read the file data in and compute its crc32 */
  int nread = 0;
  unsigned long buffer_size = 1024*1024;
  char buf[buffer_size];
  do {
    nread = kvtree_read(filename, fd, buf, buffer_size);
    if (nread > 0) {
      *crc = crc32(*crc, (const Bytef*) buf, (uInt) nread);
    }
  } while (nread == buffer_size);

  /* if we got an error, don't print anything and bailout */
  if (nread < 0) {
    kvtree_dbg(1, "Error while reading file to compute crc: %s @ %s:%d",
      filename, __FILE__, __LINE__
    );
    close(fd);
    return KVTREE_FAILURE;
  }

  /* close the file */
  kvtree_close(filename, fd);

  return KVTREE_SUCCESS;
}

/*
=========================================
Directory functions
=========================================
*/

/* recursively create directory and subdirectories */
int kvtree_mkdir(const char* dir, mode_t mode)
{
  int rc = KVTREE_SUCCESS;

  /* With dirname, either the original string may be modified or the function may return a
   * pointer to static storage which will be overwritten by the next call to dirname,
   * so we need to strdup both the argument and the return string. */

  /* extract leading path from dir = full path - basename */
  char* dircopy = strdup(dir);
  char* path    = strdup(dirname(dircopy));

  /* if we can read path or path=="." or path=="/", then there's nothing to do,
   * otherwise, try to create it */
  if (access(path, R_OK) < 0 &&
      strcmp(path,".") != 0  &&
      strcmp(path,"/") != 0)
  {
    rc = kvtree_mkdir(path, mode);
  }

  /* if we can write to path, try to create subdir within path */
  if (access(path, W_OK) == 0 && rc == KVTREE_SUCCESS) {
    int tmp_rc = mkdir(dir, mode);
    if (tmp_rc < 0) {
      if (errno == EEXIST) {
        /* don't complain about mkdir for a directory that already exists */
        kvtree_free(&dircopy);
        kvtree_free(&path);
        return KVTREE_SUCCESS;
      } else {
        kvtree_err("Creating directory: mkdir(%s, %x) path=%s errno=%d %s @ %s:%d",
          dir, mode, path, errno, strerror(errno), __FILE__, __LINE__
        );
        rc = KVTREE_FAILURE;
      }
    }
  } else {
    kvtree_err("Cannot write to directory: %s @ %s:%d",
      path, __FILE__, __LINE__
    );
    rc = KVTREE_FAILURE;
  }

  /* free our dup'ed string and return error code */
  kvtree_free(&dircopy);
  kvtree_free(&path);
  return rc;
}

/* remove directory */
int kvtree_rmdir(const char* dir)
{
  /* delete directory */
  int rc = rmdir(dir);
  if (rc < 0) {
    /* whoops, something failed when we tried to delete our directory */
    kvtree_err("Error deleting directory: %s (rmdir returned %d %s) @ %s:%d",
      dir, rc, strerror(errno), __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }
  return KVTREE_SUCCESS;
}

/* write current working directory to buf */
int kvtree_getcwd(char* buf, size_t size)
{
  int rc = KVTREE_SUCCESS;
  if (getcwd(buf, size) == NULL) {
    kvtree_abort(-1, "Problem reading current working directory (getcwd() errno=%d %s) @ %s:%d",
              errno, strerror(errno), __FILE__, __LINE__
    );
    rc = KVTREE_FAILURE;
  }
  return rc;
}

/*
=========================================
File Copy Functions
=========================================
*/

/* TODO: could perhaps use O_DIRECT here as an optimization */
/* TODO: could apply compression/decompression here */
/* copy src_file (full path) to dest_path and return new full path in dest_file */
int kvtree_file_copy(
  const char* src_file,
  const char* dst_file,
  unsigned long buf_size,
  uLong* crc)
{
  /* check that we got something for a source file */
  if (src_file == NULL || strcmp(src_file, "") == 0) {
    kvtree_err("Invalid source file @ %s:%d",
      __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }

  /* check that we got something for a destination file */
  if (dst_file == NULL || strcmp(dst_file, "") == 0) {
    kvtree_err("Invalid destination file @ %s:%d",
      __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }

  int rc = KVTREE_SUCCESS;

  /* open src_file for reading */
  int src_fd = kvtree_open(src_file, O_RDONLY);
  if (src_fd < 0) {
    kvtree_err("Opening file to copy: kvtree_open(%s) errno=%d %s @ %s:%d",
      src_file, errno, strerror(errno), __FILE__, __LINE__
    );
    return KVTREE_FAILURE;
  }

  /* open dest_file for writing */
  mode_t mode_file = kvtree_getmode(1, 1, 0);
  int dst_fd = kvtree_open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, mode_file);
  if (dst_fd < 0) {
    kvtree_err("Opening file for writing: kvtree_open(%s) errno=%d %s @ %s:%d",
      dst_file, errno, strerror(errno), __FILE__, __LINE__
    );
    kvtree_close(src_file, src_fd);
    return KVTREE_FAILURE;
  }

#if !defined(__APPLE__)
  /* TODO:
  posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED | POSIX_FADV_SEQUENTIAL)
  that tells the kernel that you don't ever need the pages
  from the file again, and it won't bother keeping them in the page cache.
  */
  posix_fadvise(src_fd, 0, 0, POSIX_FADV_DONTNEED | POSIX_FADV_SEQUENTIAL);
  posix_fadvise(dst_fd, 0, 0, POSIX_FADV_DONTNEED | POSIX_FADV_SEQUENTIAL);
#endif

  /* allocate buffer to read in file chunks */
  char* buf = (char*) malloc(buf_size);
  if (buf == NULL) {
    kvtree_err("Allocating memory: malloc(%llu) errno=%d %s @ %s:%d",
      buf_size, errno, strerror(errno), __FILE__, __LINE__
    );
    kvtree_close(dst_file, dst_fd);
    kvtree_close(src_file, src_fd);
    return KVTREE_FAILURE;
  }

  /* initialize crc values */
  if (crc != NULL) {
    *crc = crc32(0L, Z_NULL, 0);
  }

  /* write chunks */
  int copying = 1;
  while (copying) {
    /* attempt to read buf_size bytes from file */
    int nread = kvtree_read_attempt(src_file, src_fd, buf, buf_size);

    /* if we read some bytes, write them out */
    if (nread > 0) {
      /* optionally compute crc value as we go */
      if (crc != NULL) {
        *crc = crc32(*crc, (const Bytef*) buf, (uInt) nread);
      }

      /* write our nread bytes out */
      int nwrite = kvtree_write_attempt(dst_file, dst_fd, buf, nread);

      /* check for a write error or a short write */
      if (nwrite != nread) {
        /* write had a problem, stop copying and return an error */
        copying = 0;
        rc = KVTREE_FAILURE;
      }
    }

    /* assume a short read means we hit the end of the file */
    if (nread < buf_size) {
      copying = 0;
    }

    /* check for a read error, stop copying and return an error */
    if (nread < 0) {
      /* read had a problem, stop copying and return an error */
      copying = 0;
      rc = KVTREE_FAILURE;
    }
  }

  /* free buffer */
  kvtree_free(&buf);

  /* close source and destination files */
  if (kvtree_close(dst_file, dst_fd) != KVTREE_SUCCESS) {
    rc = KVTREE_FAILURE;
  }
  if (kvtree_close(src_file, src_fd) != KVTREE_SUCCESS) {
    rc = KVTREE_FAILURE;
  }

  /* unlink the file if the copy failed */
  if (rc != KVTREE_SUCCESS) {
    unlink(dst_file);
  }

  return rc;
}
