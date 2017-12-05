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

#include "kvtree.h"
#include "kvtree_util.h"

#include "stdlib.h"
#include "stdio.h"

#define TEST_PASS (0)
#define TEST_FAIL (1)

char** tests_failed;

int test_util_set_get_bytecount(){
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

int test_util_set_get_int(){
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

int test_util_set_get_unsigned_long(){
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

int test_util_set_get_str(){
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

int test_util_set_get_int64(){
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

int test_util_set_get_double(){
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

int main(int argc, char** argv){
  int num_tests = 6;
  int num_failed = 0;
  tests_failed = calloc(num_tests, sizeof(char*));

  if (test_util_set_get_bytecount() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_bytecount";
    num_failed++;
  }

  if (test_util_set_get_int() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_int";
    num_failed++;
  }

  if (test_util_set_get_unsigned_long() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_unsigned_long";
    num_failed++;
  }

  if (test_util_set_get_str() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_str";
    num_failed++;
  }

  if (test_util_set_get_int64() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_int64";
    num_failed++;
  }

  if (test_util_set_get_double() != TEST_PASS){
    tests_failed[num_failed] = "test_util_set_get_double";
    num_failed++;
  }

  printf("Ran %d tests: %d pass, %d fail.\n", num_tests, num_tests - num_failed, num_failed);
  int i;
  for(i = 0; i < num_failed; i++){
    printf("    %s FAILED.\n", tests_failed[i]);
  }

  return num_failed;
}
