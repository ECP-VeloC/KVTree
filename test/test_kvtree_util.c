/*
 * Copyright (c) 2009, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by Adam Moody <moody20@llnl.gov>.
 * LLNL-CODE-411039.
 * All rights reserved.
 * KVTree was originally part of The Scalable Checkpoint / Restart (SCR) library.
 * For details, see https://sourceforge.net/projects/scalablecr/
 * Please also read this file: LICENSE.TXT.
*/

#include "test_kvtree.h"
#include "test_kvtree_util.h"

#include <string.h>

int test_kvtree_util_set_get_bytecount(){
  int rc = TEST_PASS;
  char* key = "KEY";
  unsigned long value = 17;
  unsigned long roundtrip = 0;

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_bytecount(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_bytecount(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(roundtrip != value) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

int test_kvtree_util_set_get_int(){
  int rc = TEST_PASS;
  char* key = "KEY";
  int value = 17;
  int roundtrip = 0;

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_int(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_int(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(roundtrip != value) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

int test_kvtree_util_set_get_unsigned_long(){
  int rc = TEST_PASS;
  char* key = "KEY";
  unsigned long value = 17;
  unsigned long roundtrip = 0;

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_unsigned_long(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_unsigned_long(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(roundtrip != value) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

int test_kvtree_util_set_get_str(){
  int rc = TEST_PASS;
  char* key = "KEY";
  char*  value = "17";
  char*  roundtrip = "0";

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_str(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_str(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(strcmp(roundtrip, value)) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

int test_kvtree_util_set_get_int64(){
  int rc = TEST_PASS;
  char* key = "KEY";
  int64_t value = 17;
  int64_t roundtrip = 0;

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_int64(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_int64(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(roundtrip != value) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

int test_kvtree_util_set_get_double(){
  int rc = TEST_PASS;
  char* key = "KEY";
  double value = 17.0;
  double roundtrip = 0.0;

  kvtree* kvtree = kvtree_new();

  if(kvtree == NULL) rc = TEST_FAIL;
  if(kvtree_util_set_double(kvtree, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(kvtree_util_get_double(kvtree, key, &roundtrip) != KVTREE_SUCCESS) rc = TEST_FAIL;
  if(roundtrip != value) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  return rc;
}

void test_kvtree_util_init(){
  register_test(test_kvtree_util_set_get_bytecount, "test_kvtree_util_set_get_bytecount");
  register_test(test_kvtree_util_set_get_int, "test_kvtree_util_set_get_int");
  register_test(test_kvtree_util_set_get_str, "test_kvtree_util_set_get_str");
  register_test(test_kvtree_util_set_get_unsigned_long, "test_kvtree_util_set_get_unsigned_long");
  register_test(test_kvtree_util_set_get_int64, "test_kvtree_util_set_get_int64");
  register_test(test_kvtree_util_set_get_double, "test_kvtree_util_set_get_double");
}
