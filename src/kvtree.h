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

#ifndef KVTREE_H
#define KVTREE_H

#include <stdarg.h>
#include <sys/types.h>

/* need at least version 8.5 of queue.h from Berkeley */
#include "queue.h"

/*
=========================================
This file defines the data structure for a hash,
which is an unordered list of elements,
where each element contains a key (char string)
and a pointer to another hash.
=========================================
*/
#define KVTREE_MAJOR "0"
#define KVTREE_MINOR "0"
#define KVTREE_PATCH "1"
#define KVTREE_VERSION "0.0.1"
#define KVTREE_SUCCESS (0)
#define KVTREE_MAX_FILENAME (1024)

/*
=========================================
Sort directions for sorting keys in hash
========================================
*/

#define KVTREE_SORT_ASCENDING  (0)
#define KVTREE_SORT_DESCENDING (1)

/*
=========================================
Define common hash key strings
========================================
*/

/* generic hash keys */
#define KVTREE_KEY_DATASET   ("DSET")
#define KVTREE_KEY_PATH      ("PATH")
#define KVTREE_KEY_SEGMENT   ("SEG")
#define KVTREE_KEY_CONTAINER ("CTR")
#define KVTREE_KEY_ID        ("ID")
#define KVTREE_KEY_NAME      ("NAME")
#define KVTREE_KEY_SIZE      ("SIZE")
#define KVTREE_KEY_OFFSET    ("OFFSET")
#define KVTREE_KEY_LENGTH    ("LENGTH")
#define KVTREE_KEY_RANK      ("RANK")
#define KVTREE_KEY_RANKS     ("RANKS")
#define KVTREE_KEY_DIRECTORY ("DIR")
#define KVTREE_KEY_FILE      ("FILE")
#define KVTREE_KEY_FILES     ("FILES")
#define KVTREE_KEY_META      ("META")
#define KVTREE_KEY_COMPLETE  ("COMPLETE")
#define KVTREE_KEY_CRC       ("CRC")

/* these keys are kept in hashes stored in files for long periods of time,
 * thus we associate a version number with them in order to read old files */
#define KVTREE_SUMMARY_KEY_VERSION ("VERSION")

#define KVTREE_SUMMARY_FILE_VERSION_5 (5)
#define KVTREE_SUMMARY_5_KEY_CKPT      ("CKPT")
#define KVTREE_SUMMARY_5_KEY_RANK      ("RANK")
#define KVTREE_SUMMARY_5_KEY_RANKS     ("RANKS")
#define KVTREE_SUMMARY_5_KEY_COMPLETE  ("COMPLETE")
#define KVTREE_SUMMARY_5_KEY_FILE      ("FILE")
#define KVTREE_SUMMARY_5_KEY_FILES     ("FILES")
#define KVTREE_SUMMARY_5_KEY_SIZE      ("SIZE")
#define KVTREE_SUMMARY_5_KEY_CRC       ("CRC")
#define KVTREE_SUMMARY_5_KEY_NOFETCH   ("NOFETCH")

#define KVTREE_SUMMARY_FILE_VERSION_6 (6)
#define KVTREE_SUMMARY_6_KEY_DATASET   ("DSET")
#define KVTREE_SUMMARY_6_KEY_RANK2FILE ("RANK2FILE")
#define KVTREE_SUMMARY_6_KEY_LEVEL     ("LEVEL")
#define KVTREE_SUMMARY_6_KEY_RANK      ("RANK")
#define KVTREE_SUMMARY_6_KEY_RANKS     ("RANKS")
#define KVTREE_SUMMARY_6_KEY_COMPLETE  ("COMPLETE")
#define KVTREE_SUMMARY_6_KEY_FILE      ("FILE")
#define KVTREE_SUMMARY_6_KEY_FILES     ("FILES")
#define KVTREE_SUMMARY_6_KEY_SIZE      ("SIZE")
#define KVTREE_SUMMARY_6_KEY_CRC       ("CRC")
#define KVTREE_SUMMARY_6_KEY_NOFETCH   ("NOFETCH")
#define KVTREE_SUMMARY_6_KEY_CONTAINER ("CTR")
#define KVTREE_SUMMARY_6_KEY_SEGMENT   ("SEG")
#define KVTREE_SUMMARY_6_KEY_ID        ("ID")
#define KVTREE_SUMMARY_6_KEY_LENGTH    ("LENGTH")
#define KVTREE_SUMMARY_6_KEY_OFFSET    ("OFFSET")

#define KVTREE_INDEX_KEY_VERSION ("VERSION")

#define KVTREE_INDEX_FILE_VERSION_1 (1)
#define KVTREE_INDEX_1_KEY_NAME      ("NAME")
#define KVTREE_INDEX_1_KEY_DIR       ("DIR")
#define KVTREE_INDEX_1_KEY_CKPT      ("CKPT")
#define KVTREE_INDEX_1_KEY_DATASET   ("DSET")
#define KVTREE_INDEX_1_KEY_COMPLETE  ("COMPLETE")
#define KVTREE_INDEX_1_KEY_FETCHED   ("FETCHED")
#define KVTREE_INDEX_1_KEY_FLUSHED   ("FLUSHED")
#define KVTREE_INDEX_1_KEY_FAILED    ("FAILED")
#define KVTREE_INDEX_1_KEY_CURRENT   ("CURRENT")

/* the rest of these hash keys are only used in memory or in files
 * that live for the life of the job, thus backwards compatibility is not needed */
#define KVTREE_FLUSH_KEY_DATASET  ("DATASET")
#define KVTREE_FLUSH_KEY_LOCATION ("LOCATION")
#define KVTREE_FLUSH_KEY_LOCATION_CACHE    ("CACHE")
#define KVTREE_FLUSH_KEY_LOCATION_PFS      ("PFS")
#define KVTREE_FLUSH_KEY_LOCATION_FLUSHING ("FLUSHING")
#define KVTREE_FLUSH_KEY_LOCATION_SYNC_FLUSHING ("SYNC_FLUSHING")
#define KVTREE_FLUSH_KEY_DIRECTORY ("DIR")
#define KVTREE_FLUSH_KEY_NAME      ("NAME")
#define KVTREE_FLUSH_KEY_CKPT      ("CKPT")
#define KVTREE_FLUSH_KEY_OUTPUT    ("OUTPUT")

#define KVTREE_SCAVENGE_KEY_PRESERVE  ("PRESERVE")
#define KVTREE_SCAVENGE_KEY_CONTAINER ("CONTAINER")
#define KVTREE_SCAVENGE_KEY_PARTNER   ("PARTNER")

#define KVTREE_NODES_KEY_NODES ("NODES")

/* transfer file keys */
#define KVTREE_TRANSFER_KEY_FILES       ("FILES")
#define KVTREE_TRANSFER_KEY_DESTINATION ("DESTINATION")
#define KVTREE_TRANSFER_KEY_SIZE        ("SIZE")
#define KVTREE_TRANSFER_KEY_WRITTEN     ("WRITTEN")
#define KVTREE_TRANSFER_KEY_BW          ("BW")
#define KVTREE_TRANSFER_KEY_PERCENT     ("PERCENT")

#define KVTREE_TRANSFER_KEY_COMMAND ("COMMAND")
#define KVTREE_TRANSFER_KEY_COMMAND_RUN  ("RUN")
#define KVTREE_TRANSFER_KEY_COMMAND_STOP ("STOP")
#define KVTREE_TRANSFER_KEY_COMMAND_EXIT ("EXIT")

#define KVTREE_TRANSFER_KEY_STATE ("STATE")
#define KVTREE_TRANSFER_KEY_STATE_RUN  ("RUNNING")
#define KVTREE_TRANSFER_KEY_STATE_STOP ("STOPPED")
#define KVTREE_TRANSFER_KEY_STATE_EXIT ("EXITING")

#define KVTREE_TRANSFER_KEY_FLAG ("FLAG")
#define KVTREE_TRANSFER_KEY_FLAG_DONE ("DONE")

/* ckpt config file keys */
#define KVTREE_CONFIG_KEY_GROUPDESC  ("GROUPS")
#define KVTREE_CONFIG_KEY_STOREDESC  ("STORE")
#define KVTREE_CONFIG_KEY_CACHEDESC  ("CACHE")
#define KVTREE_CONFIG_KEY_COUNT      ("COUNT")
#define KVTREE_CONFIG_KEY_NAME       ("NAME")
#define KVTREE_CONFIG_KEY_BASE       ("BASE")
#define KVTREE_CONFIG_KEY_STORE      ("STORE")
#define KVTREE_CONFIG_KEY_SIZE       ("SIZE")
#define KVTREE_CONFIG_KEY_GROUP      ("GROUP")

#define KVTREE_CONFIG_KEY_CKPTDESC   ("CKPT")
#define KVTREE_CONFIG_KEY_ENABLED    ("ENABLED")
#define KVTREE_CONFIG_KEY_INDEX      ("INDEX")
#define KVTREE_CONFIG_KEY_INTERVAL   ("INTERVAL")
#define KVTREE_CONFIG_KEY_OUTPUT     ("OUTPUT")
#define KVTREE_CONFIG_KEY_DIRECTORY  ("DIRECTORY")
#define KVTREE_CONFIG_KEY_TYPE       ("TYPE")
#define KVTREE_CONFIG_KEY_FAIL_GROUP ("FAIL_GROUP")
#define KVTREE_CONFIG_KEY_SET_SIZE   ("SET_SIZE")
#define KVTREE_CONFIG_KEY_GROUPS     ("GROUPS")
#define KVTREE_CONFIG_KEY_GROUP_ID   ("GROUP_ID")
#define KVTREE_CONFIG_KEY_GROUP_SIZE ("GROUP_SIZE")
#define KVTREE_CONFIG_KEY_GROUP_RANK ("GROUP_RANK")
#define KVTREE_CONFIG_KEY_MKDIR      ("MKDIR")
#define KVTREE_CONFIG_KEY_TYPE       ("TYPE")
#define KVTREE_CONFIG_KEY_VIEW       ("VIEW")

#define KVTREE_DATASET_KEY_ID       ("ID")
#define KVTREE_DATASET_KEY_USER     ("USER")
#define KVTREE_DATASET_KEY_JOBNAME  ("JOBNAME")
#define KVTREE_DATASET_KEY_NAME     ("NAME")
#define KVTREE_DATASET_KEY_SIZE     ("SIZE")
#define KVTREE_DATASET_KEY_FILES    ("FILES")
#define KVTREE_DATASET_KEY_CREATED  ("CREATED")
#define KVTREE_DATASET_KEY_JOBID    ("JOBID")
#define KVTREE_DATASET_KEY_CLUSTER  ("CLUSTER")
#define KVTREE_DATASET_KEY_CKPT     ("CKPT")
#define KVTREE_DATASET_KEY_COMPLETE ("COMPLETE")
#define KVTREE_DATASET_KEY_FLAG_CKPT   ("FLAG_CKPT")
#define KVTREE_DATASET_KEY_FLAG_OUTPUT ("FLAG_OUTPUT")

#define KVTREE_META_KEY_CKPT     ("CKPT")
#define KVTREE_META_KEY_RANKS    ("RANKS")
#define KVTREE_META_KEY_RANK     ("RANK")
#define KVTREE_META_KEY_ORIG     ("ORIG")
#define KVTREE_META_KEY_PATH     ("PATH")
#define KVTREE_META_KEY_NAME     ("NAME")
#define KVTREE_META_KEY_FILE     ("FILE")
#define KVTREE_META_KEY_SIZE     ("SIZE")
#define KVTREE_META_KEY_TYPE     ("TYPE")
#define KVTREE_META_KEY_TYPE_USER ("USER")
#define KVTREE_META_KEY_TYPE_XOR  ("XOR")
#define KVTREE_META_KEY_CRC      ("CRC")
#define KVTREE_META_KEY_COMPLETE ("COMPLETE")

#define KVTREE_KEY_COPY_XOR_CHUNK   ("CHUNK")
#define KVTREE_KEY_COPY_XOR_DATASET ("DSET")
#define KVTREE_KEY_COPY_XOR_CURRENT ("CURRENT")
#define KVTREE_KEY_COPY_XOR_PARTNER ("PARTNER")
#define KVTREE_KEY_COPY_XOR_FILES   ("FILES")
#define KVTREE_KEY_COPY_XOR_FILE    ("FILE")
#define KVTREE_KEY_COPY_XOR_RANKS   ("RANKS")
#define KVTREE_KEY_COPY_XOR_RANK    ("RANK")
#define KVTREE_KEY_COPY_XOR_GROUP   ("GROUP")
#define KVTREE_KEY_COPY_XOR_GROUP_RANKS ("RANKS")
#define KVTREE_KEY_COPY_XOR_GROUP_RANK  ("RANK")

/*
=========================================
Define hash and element structures
=========================================
*/

/* define the structure for the head of a hash */
struct kvtree_elem_struct;
LIST_HEAD(kvtree_struct, kvtree_elem_struct);

/* define the structure for an element of a hash */
struct kvtree_elem_struct {
  char* key;
  struct kvtree_struct* hash;
  LIST_ENTRY(kvtree_elem_struct) pointers;
};

typedef struct kvtree_struct      kvtree;
typedef struct kvtree_elem_struct kvtree_elem;

/*
=========================================
Allocate and delete hash objects
=========================================
*/

/* allocates a new hash */
kvtree* kvtree_new(void);

/* frees a hash */
int kvtree_delete(kvtree** ptr_hash);

/*
=========================================
size, get, set, unset, and merge functions
=========================================
*/

/* return size of hash (number of keys) */
int kvtree_size(const kvtree* hash);

/* given a hash and a key, return the hash associated with key, returns NULL if not found */
kvtree* kvtree_get(const kvtree* hash, const char* key);

/* given a hash, a key, and a hash value, set (or reset) the key's hash */
kvtree* kvtree_set(kvtree* hash, const char* key, kvtree* hash_value);

/* given a hash and a key, extract and return hash for specified key, returns NULL if not found */
kvtree* kvtree_extract(kvtree* hash, const char* key);

/* given a hash and a key, extract and delete any matching element */
int kvtree_unset(kvtree* hash, const char* key);

/* unset all values in the hash, but don't delete it */
int kvtree_unset_all(kvtree* hash);

/* merges (copies) elements from hash2 into hash1 */
int kvtree_merge(kvtree* hash1, const kvtree* hash2);

/* traverse the given hash using a printf-like format string setting an arbitrary list of keys
 * to set (or reset) the hash associated with the last key */
kvtree* kvtree_setf(kvtree* hash, kvtree* hash_value, const char* format, ...);

/* same as above, but simply returns the hash associated with the list of keys */
kvtree* kvtree_getf(const kvtree* hash, const char* format, ...);

/* sort the hash assuming the keys are strings */
int kvtree_sort(kvtree* hash, int direction);

/* sort the hash assuming the keys are ints */
int kvtree_sort_int(kvtree* hash, int direction);

/* return list of keys in hash as integers, caller must free list */
int kvtree_list_int(const kvtree* hash, int* num, int** list);

/*
=========================================
get, set, and unset hashes using a key/value pair
=========================================
*/

/* shortcut to create a key and subkey in a hash with one call */
kvtree* kvtree_set_kv(kvtree* hash, const char* key, const char* val);

/* same as kvtree_set_kv, but with the subkey specified as an int */
kvtree* kvtree_set_kv_int(kvtree* hash, const char* key, int val);

/* shortcut to get hash assocated with the subkey of a key in a hash with one call */
kvtree* kvtree_get_kv(const kvtree* hash, const char* key, const char* val);

/* same as kvtree_get_kv, but with the subkey specified as an int */
kvtree* kvtree_get_kv_int(const kvtree* hash, const char* key, int val);

/* unset subkey under key, and if that removes the only element for key, unset key as well */
int kvtree_unset_kv(kvtree* hash, const char* key, const char* val);

/* same as kvtree_unset_kv, but with the subkey specified as an int */
int kvtree_unset_kv_int(kvtree* hash, const char* key, int val);

/*
=========================================
Hash element functions
=========================================
*/

/* returns the first element for a given hash */
kvtree_elem* kvtree_elem_first(const kvtree* hash);

/* given a hash element, returns the next element */
kvtree_elem* kvtree_elem_next(const kvtree_elem* elem);

/* returns a pointer to the key of the specified element */
char* kvtree_elem_key(const kvtree_elem* elem);

/* same as kvtree_elem_key, but converts the key as an int (returns 0 if key is not defined) */
int kvtree_elem_key_int(const kvtree_elem* elem);

/* returns a pointer to the hash of the specified element */
kvtree* kvtree_elem_hash(const kvtree_elem* elem);

/* given a hash and a key, find first matching element and return its address, returns NULL if not found */
kvtree_elem* kvtree_elem_get(const kvtree* hash, const char* key);

/* given a hash and a key, return a pointer to the key of the first element of that key's hash */
char* kvtree_elem_get_first_val(const kvtree* hash, const char* key);

/* given a hash and a key, find first matching element, remove it from the hash, and return it */
kvtree_elem* kvtree_elem_extract(kvtree* hash, const char* key);

/* given a hash and a key, find first matching element, remove it from the hash, and return it */
kvtree_elem* kvtree_elem_extract_int(kvtree* hash, int key);

/* extract element from hash given the hash and the address of the element */
kvtree_elem* kvtree_elem_extract_by_addr(kvtree* hash, kvtree_elem* elem);

char* kvtree_get_val(const kvtree* hash, const char* key);

/*
=========================================
Pack and unpack hash and elements into a char buffer
=========================================
*/

/* computes the number of bytes needed to pack the given hash */
size_t kvtree_pack_size(const kvtree* hash);

/* packs the given hash into specified buf and returns the number of bytes written */
size_t kvtree_pack(char* buf, const kvtree* hash);

/* unpacks hash from specified buffer into given hash object and returns the number of bytes read */
size_t kvtree_unpack(const char* buf, kvtree* hash);

/*
=========================================
Read and write hash to a file
=========================================
*/

/* persist hash in newly allocated buffer,
 * return buffer address and size to be freed by caller */
int kvtree_write_persist(void** ptr_buf, size_t* ptr_size, const kvtree* hash);

/* executes logic of kvtree_has_write with opened file descriptor */
ssize_t kvtree_write_fd(const char* file, int fd, const kvtree* hash);

/* executes logic of kvtree_read using an opened file descriptor */
ssize_t kvtree_read_fd(const char* file, int fd, kvtree* hash);

/* write the given hash to specified file */
int kvtree_write_file(const char* file, const kvtree* hash);

/* opens specified file and reads in a hash storing its contents in the given hash object */
int kvtree_read_file(const char* file, kvtree* hash);

/* given a filename and hash, lock/open/read/close/unlock the file storing its contents in the hash */
int kvtree_read_with_lock(const char* file, kvtree* hash);

/* given a filename and hash, lock the file, open it, and read it into hash, set fd to the opened file descriptor */
int kvtree_lock_open_read(const char* file, int* fd, kvtree* hash);

/* given a filename, an opened file descriptor, and a hash, overwrite file with hash, close, and unlock file */
int kvtree_write_close_unlock(const char* file, int* fd, const kvtree* hash);

/*
=========================================
Print hash and elements to stdout for debugging
=========================================
*/

/* prints specified hash to stdout for debugging */
int kvtree_print(const kvtree* hash, int indent);

/* logs specified hash for debugging */
int kvtree_log(const kvtree* hash, int log_level, int indent);

#endif
