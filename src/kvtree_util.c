/* Implements some common tasks for operations on kvtrees */

#include "kvtree.h"
#include "kvtree_util.h"
#include "kvtree_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>

/* need at least version 8.5 of queue.h from Berkeley */
#include "queue.h"

int kvtree_util_set_bytecount(kvtree* hash, const char* key, unsigned long count)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %lu", key, count);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_crc32(kvtree* hash, const char* key, uLong crc)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %#x", key, (uint32_t) crc);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_int(kvtree* hash, const char* key, int value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_set_kv_int(hash, key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_unsigned_long(kvtree* hash, const char* key, unsigned long value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %lu", key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_str(kvtree* hash, const char* key, const char* value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_set_kv(hash, key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_int64(kvtree* hash, const char* key, int64_t value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %lld", key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_double(kvtree* hash, const char* key, double value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %f", key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_set_ptr(kvtree* hash, const char* key, void* value)
{
  /* first, unset any current setting */
  kvtree_unset(hash, key);

  /* then set the new value */
  kvtree_setf(hash, NULL, "%s %p", key, value);

  return KVTREE_SUCCESS;
}

int kvtree_util_get_bytecount(const kvtree* hash, const char* key, unsigned long* val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    *val = strtoul(val_str, NULL, 0);
    rc = KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_crc32(const kvtree* hash, const char* key, uLong* val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    *val = (uLong) strtoul(val_str, NULL, 0);
    rc = KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_int(const kvtree* hash, const char* key, int* value)
{
  int rc = KVTREE_FAILURE;

  char* val_str = kvtree_elem_get_first_val(hash, key);
  if (val_str != NULL) {
    *value = atoi(val_str);
    return KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_unsigned_long(const kvtree* hash, const char* key, unsigned long* val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    *val = strtoul(val_str, NULL, 0);
    rc = KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_str(const kvtree* hash, const char* key, char** val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    *val = val_str;
    rc = KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_int64(const kvtree* hash, const char* key, int64_t* val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    *val = (int64_t) strtoll(val_str, NULL, 0);
    rc = KVTREE_SUCCESS;
  }

  return rc;
}

int kvtree_util_get_double(const kvtree* hash, const char* key, double* val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    double val_tmp;
    if (kvtree_atod(val_str, &val_tmp) == KVTREE_SUCCESS) {
      *val = val_tmp;
      rc = KVTREE_SUCCESS;
    }
  }

  return rc;
}

int kvtree_util_get_ptr(const kvtree* hash, const char* key, void** val)
{
  int rc = KVTREE_FAILURE;

  /* check whether this key is even set */
  char* val_str = kvtree_get_val(hash, key);
  if (val_str != NULL) {
    /* convert the key string */
    void* val_tmp;
    int sscanf_rc = sscanf(val_str, "%p", &val_tmp);
    if (sscanf_rc == 1) {
      *val = val_tmp;
      rc = KVTREE_SUCCESS;
    }
  }

  return rc;
}

/*
 * Given a prefix passed to a kvtree_write_gather() (like "/tmp/rank2file",
 * read all the rank data from all the scattered subfiles, and construct a
 * kvtree in 'data'.  'data' will look something like:
 *
 *       26
 *         FILE
 *           ckpt.1/rank_26.ckpt
 *       24
 *         FILE
 *           ckpt.1/rank_24.ckpt
 *       25
 *         FILE
 *           ckpt.1/rank_25.ckpt
 *       18
 *         FILE
 *           ckpt.1/rank_18.ckpt
 *       ...
 *
 *  ... with each top-level element being the rank number.
 */
int kvtree_read_scatter_single(const char* prefix, kvtree* data)
{
  /*
   * Read in high level kvtree.
   */
  kvtree* final_tree = kvtree_new();
  int rc = kvtree_read_file(prefix, final_tree);
  if (rc != KVTREE_SUCCESS) {
    kvtree_delete(&final_tree);
    return rc;
  }

  /*
   * Get the total number of ranks to expect.
   */
  unsigned long expected_ranks;
  rc = kvtree_util_get_unsigned_long(final_tree, "RANKS", &expected_ranks);
  if (rc != KVTREE_SUCCESS) {
    kvtree_delete(&final_tree);
    return rc;
  }
  kvtree_delete(&final_tree);

  /* Start construction of the final tree that we will return */
  final_tree = kvtree_new();

  /* Look at all the subfiles with our prefix and read in the file lists */
  char* prefix_dir_copy = strdup(prefix);
  char* prefix_dir = dirname(prefix_dir_copy);

  char* prefix_file_copy = strdup(prefix);
  char* prefix_file = basename(prefix_file_copy);

  DIR* d = opendir(prefix_dir);
  if (!d) {
    rc = KVTREE_FAILURE;
    goto end;
  }

  char pattern[PATH_MAX];
  sprintf(pattern, "%s.%%lu.%%lu", prefix_file);
  pattern[sizeof(pattern) - 1] = '\0';

  /* For each file/dir in our prefix dir */
  unsigned long actual_ranks = 0;
  struct dirent *dir;
  while ((dir = readdir(d)) != NULL) {
    /*
     * Search for any file starting with "prefix_file."
     * like "rank2file.".
     */

    /* Is this one of our subfiles?  It should have .x.y in the extension */
    unsigned long num1, num2;
    rc = sscanf(dir->d_name, pattern, &num1, &num2);
    if (rc == 2) {  /* Did we get both numbers in the extension? */
      /* subfile matches */

      /* Construct the full path to the subfile kvtree */
      char tmp[PATH_MAX];
      memset(tmp, 0, sizeof(tmp));
      snprintf(tmp, sizeof(tmp), "%s/%s", prefix_dir, dir->d_name);
      tmp[sizeof(tmp) - 1] = '\0';

      /* Read in the subfile */
      kvtree* subfile_tree = kvtree_new();
      rc = kvtree_read_file(tmp, subfile_tree);
      if (rc == KVTREE_SUCCESS) {
        /* Each leaf file we want will have LEVEL=0 */
        unsigned long level;
        rc = kvtree_util_get_unsigned_long(subfile_tree, "LEVEL", &level);
        if (rc == KVTREE_SUCCESS && level == 0) {
          /* Break off and remove the "RANK" subtree from the rank2file tree */
          kvtree* subfile_rank_tree = kvtree_extract(subfile_tree, "RANK");
          if (subfile_rank_tree) {
            if (kvtree_merge(final_tree, subfile_rank_tree) == KVTREE_SUCCESS) {
              actual_ranks += kvtree_size(subfile_rank_tree);
            }
          }
        }
      }

      kvtree_delete(&subfile_tree);
    }
  }

  closedir(d);
  kvtree_merge(data, final_tree);
  rc = KVTREE_SUCCESS;

end:
  kvtree_free(&prefix_dir_copy);
  kvtree_free(&prefix_file_copy);
  kvtree_delete(&final_tree);
  if (rc != KVTREE_SUCCESS || actual_ranks != expected_ranks) {
    return KVTREE_FAILURE;
  }

  return KVTREE_SUCCESS;
}
