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

#ifndef KVTREE_IO_H
#define KVTREE_IO_H

#include <config.h>
#include <stdarg.h>
#include <sys/types.h>

/* compute crc32 */
#include <zlib.h>

#ifndef KVTREE_MAX_LINE
#define KVTREE_MAX_LINE (1024)
#endif

/* adds byte swapping routines */
#if defined(__APPLE__)
#include "machine/endian.h"
#else
#include "endian.h"
#endif

#ifdef HAVE_BYTESWAP_H
#include "byteswap.h"
#else
#include "kvtree_byteswap.h"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#ifdef HAVE_BYTESWAP_H
# define kvtree_ntoh16(x) bswap_16(x)
# define kvtree_ntoh32(x) bswap_32(x)
# define kvtree_ntoh64(x) bswap_64(x)
# define kvtree_hton16(x) bswap_16(x)
# define kvtree_hton32(x) bswap_32(x)
# define kvtree_hton64(x) bswap_64(x)
#else
# define kvtree_ntoh16(x) kvtree_bswap_16(x)
# define kvtree_ntoh32(x) kvtree_bswap_32(x)
# define kvtree_ntoh64(x) kvtree_bswap_64(x)
# define kvtree_hton16(x) kvtree_bswap_16(x)
# define kvtree_hton32(x) kvtree_bswap_32(x)
# define kvtree_hton64(x) kvtree_bswap_64(x)
#endif
#else
# define kvtree_ntoh16(x) (x)
# define kvtree_ntoh32(x) (x)
# define kvtree_ntoh64(x) (x)
# define kvtree_hton16(x) (x)
# define kvtree_hton32(x) (x)
# define kvtree_hton64(x) (x)
#endif

#define KVTREE_OPEN_TRIES (5)
#define KVTREE_OPEN_USLEEP (100)

/*
=========================================
Basic File I/O
=========================================
*/

/* returns user's current mode as determine by his umask */
mode_t kvtree_getmode(int read, int write, int execute);

/* open file with specified flags and mode, retry open a few times on failure */
int kvtree_open(const char* file, int flags, ...);

/* close file with an fsync */
int kvtree_close(const char* file, int fd);

/* get and release file locks */
int kvtree_file_lock_read(const char* file, int fd);
int kvtree_file_lock_write(const char* file, int fd);
int kvtree_file_unlock(const char* file, int fd);

/* opens specified file and waits on for an exclusive lock before returning the file descriptor */
int kvtree_open_with_lock(const char* file, int flags, mode_t mode);

/* unlocks the specified file descriptor and then closes the file */
int kvtree_close_with_unlock(const char* file, int fd);

/* seek file descriptor to specified position */
int kvtree_lseek(const char* file, int fd, off_t pos, int whence);

/* reliable read from opened file descriptor (retries, if necessary, until hard error) */
ssize_t kvtree_read(const char* file, int fd, void* buf, size_t size);

/* reliable write to opened file descriptor (retries, if necessary, until hard error) */
ssize_t kvtree_write(const char* file, int fd, const void* buf, size_t size);

/* make a good attempt to read from file (retries, if necessary, return error if fail) */
ssize_t kvtree_read_attempt(const char* file, int fd, void* buf, size_t size);

/* make a good attempt to write to file (retries, if necessary, return error if fail) */
ssize_t kvtree_write_attempt(const char* file, int fd, const void* buf, size_t size);

/* read line from file into buf with given size */
ssize_t kvtree_read_line(const char* file, int fd, char* buf, size_t size);

/* write a formatted string to specified file descriptor */
ssize_t kvtree_writef(const char* file, int fd, const char* format, ...);

/* logically concatenate n opened files and read count bytes from this logical file into buf starting
 * from offset, pad with zero on end if missing data */
int kvtree_read_pad_n(
  int n,
  char** files,
  int* fds,
  char* buf,
  unsigned long count,
  unsigned long offset,
  unsigned long* filesizes
);

/* write to an array of open files with known filesizes and treat them as one single large file */
int kvtree_write_pad_n(
  int n,
  char** files,
  int* fds,
  char* buf,
  unsigned long count,
  unsigned long offset,
  unsigned long* filesizes
);

/* given a filename, return number of bytes in file */
unsigned long kvtree_file_size(const char* file);

/* tests whether the file or directory exists */
int kvtree_file_exists(const char* file);

/* tests whether the file or directory is readable */
int kvtree_file_is_readable(const char* file);

/* tests whether the file or directory is writeable */
int kvtree_file_is_writeable(const char* file);

/* delete a file */
int kvtree_file_unlink(const char* file);

/* opens, reads, and computes the crc32 value for the given filename */
int kvtree_crc32(const char* filename, uLong* crc);

/*
=========================================
Directory functions
=========================================
*/

/* recursively create directory and subdirectories */
int kvtree_mkdir(const char* dir, mode_t mode);

/* remove directory */
int kvtree_rmdir(const char* dir);

/* write current working directory to buf */
int kvtree_getcwd(char* buf, size_t size);

/*
=========================================
File Copy Functions
=========================================
*/

int kvtree_file_copy(
  const char* src_file,
  const char* dst_file,
  unsigned long buf_size,
  uLong* crc
);

/*
=========================================
File compression functions
=========================================
*/

/* compress the specified file using blocks of size block_size and store as file_dst */
int kvtree_compress_in_place(const char* file_src, const char* file_dst, unsigned long block_size, int level);

/* uncompress the specified file and store as file_dst */
int kvtree_uncompress_in_place(const char* file_src, const char* file_dst);

/* compress the specified file using blocks of size block_size and store as file_dst */
int kvtree_compress(const char* file_src, const char* file_dst, unsigned long block_size, int level);

/* uncompress the specified file and store as file_dst */
int kvtree_uncompress(const char* file_src, const char* file_dst);

#endif
