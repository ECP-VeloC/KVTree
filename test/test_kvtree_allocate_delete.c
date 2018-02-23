/*
* Copyright (c) 2018, Lawrence Livermore National Security, LLC.
* Produced at the Lawrence Livermore National Laboratory.
*
* Copyright (c) 2018, UChicago Argonne LLC,
*   operator of Argonne National Laboratory
*
* LLNL-CODE-745961
* All rights reserved.
*
* This is the license for KVTree.
* For details, see https://github.com/LLNL/KVTree
* Please read the LICENSE file for full license text.
*/

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
