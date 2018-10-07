#include "test_kvtree.h"
#include "test_kvtree_kv.h"

#include <string.h>
#include <stdlib.h>

#define TEST_PASS (0)
#define TEST_FAIL (1)

int test_kvtree_kv(){
  int rc = TEST_PASS;
  char* key = "key";
  char* value = "value";

  kvtree* kvt = kvtree_new();
  if (kvt == NULL) rc = TEST_FAIL;

  kvtree* val = kvtree_set_kv(kvt, key, value);
  if (val == NULL) rc = TEST_FAIL;
  char* valstr = kvtree_elem_get_first_val(kvt, key);
  if(strcmp(valstr, value)) rc = TEST_FAIL;

  int size = kvtree_size(kvt);
  if (size != 1) rc = TEST_FAIL;

  kvtree* get = kvtree_get_kv(kvt, key, value);
  if (get != val) rc = TEST_FAIL;

  if(kvtree_unset_kv(kvt, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  size = kvtree_size(kvt);
  if(size != 0) rc = TEST_FAIL;

  kvtree_delete(&kvt);
  return rc;
}

int test_kvtree_kv_nested(){
  int rc = TEST_PASS;
  char* key = "key";
  char* value = "value";
  char* key2 = "key2";
  char* value2 = "value2";

  kvtree* kvt = kvtree_new();
  if (kvt == NULL) rc = TEST_FAIL;

  kvtree* val = kvtree_set_kv(kvt, key, value);
  if (val == NULL) rc = TEST_FAIL;
  char* valstr = kvtree_elem_get_first_val(kvt, key);
  if(strcmp(valstr, value)) rc = TEST_FAIL;

  int size = kvtree_size(kvt);
  if (size != 1) rc = TEST_FAIL;

  int valsize = kvtree_size(val);
  if (valsize != 0) rc = TEST_FAIL;

  kvtree* val2 = kvtree_set_kv(val, key2, value2);
  if (val2 == NULL) rc = TEST_FAIL;
  char* val2str = kvtree_elem_get_first_val(val, key2);
  if(strcmp(val2str, value2)) rc = TEST_FAIL;

  valsize = kvtree_size(val);
  if (valsize != 1) rc = TEST_FAIL;

  kvtree* get = kvtree_get_kv(kvt, key, value);
  if (get != val) rc = TEST_FAIL;

  if(kvtree_unset_kv(kvt, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  size = kvtree_size(kvt);
  if(size != 0) rc = TEST_FAIL;

  kvtree_delete(&kvt);
  return rc;
}

int test_kvtree_kv_multiple(){
  int rc = TEST_PASS;
  char* key = "key";
  char* value = "value";
  char* key2 = "key2";
  char* value2 = "value2";

  kvtree* kvt = kvtree_new();
  if (kvt == NULL) rc = TEST_FAIL;

  kvtree* val = kvtree_set_kv(kvt, key, value);
  if (val == NULL) rc = TEST_FAIL;
  char* valstr = kvtree_elem_get_first_val(kvt, key);
  if(strcmp(valstr, value)) rc = TEST_FAIL;

  kvtree* val2 = kvtree_set_kv(kvt, key2, value2);
  if (val == NULL) rc = TEST_FAIL;
  char* val2str = kvtree_elem_get_first_val(kvt, key2);
  if(strcmp(val2str, value2)) rc = TEST_FAIL;

  int size = kvtree_size(kvt);
  if (size != 2) rc = TEST_FAIL;

  kvtree* get = kvtree_get_kv(kvt, key, value);
  if (get != val) rc = TEST_FAIL;

  kvtree* get2 = kvtree_get_kv(kvt, key2, value2);
  if (get2 != val2) rc = TEST_FAIL;

  if(kvtree_unset_kv(kvt, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  size = kvtree_size(kvt);
  if(size != 1) rc = TEST_FAIL;

  kvtree* get3 = kvtree_get_kv(kvt, key2, value2);
  if (get3 != val2) rc = TEST_FAIL;

  kvtree_delete(&kvt);
  return rc;
}

int test_kvtree_kv_int(){
  int rc = TEST_PASS;
  char* key = "key";
  int value = 17;

  kvtree* kvt = kvtree_new();
  if (kvt == NULL) rc = TEST_FAIL;

  kvtree* val = kvtree_set_kv_int(kvt, key, value);
  if (val == NULL) rc = TEST_FAIL;
  char* valstr = kvtree_elem_get_first_val(kvt, key);
  int valint = atoi(valstr);
  if (valint != value) rc = TEST_FAIL;

  int size = kvtree_size(kvt);
  if (size != 1) rc = TEST_FAIL;

  kvtree* get = kvtree_get_kv_int(kvt, key, value);
  if (get != val) rc = TEST_FAIL;

  if(kvtree_unset_kv_int(kvt, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
  size = kvtree_size(kvt);
  if(size != 0) rc = TEST_FAIL;

  kvtree_delete(&kvt);
  return rc;
}

void test_kvtree_kv_init(){
  register_test(test_kvtree_kv, "test_kvtree_kv");
  register_test(test_kvtree_kv_nested, "test_kvtree_kv_nested");
  register_test(test_kvtree_kv_multiple, "test_kvtree_kv_multiple");
  register_test(test_kvtree_kv_int, "test_kvtree_kv_int");
}
