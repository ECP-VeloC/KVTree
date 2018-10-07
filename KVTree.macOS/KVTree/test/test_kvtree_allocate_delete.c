#include "test_kvtree.h"
#include "test_kvtree_allocate_delete.h"

#include <string.h>

#define TEST_PASS (0)
#define TEST_FAIL (1)

int test_kvtree_allocate_delete(){
  int rc = TEST_PASS;

  kvtree* kvtree = kvtree_new();
  if (kvtree == NULL) rc = TEST_FAIL;

  kvtree_delete(&kvtree);
  if (kvtree != NULL) rc = TEST_FAIL;

  return rc;
}

void test_kvtree_allocate_delete_init(){
  register_test(test_kvtree_allocate_delete, "test_kvtree_allocate_delete");
}
