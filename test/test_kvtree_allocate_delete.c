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
