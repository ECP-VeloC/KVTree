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

#ifndef TEST_KVTREE_UTIL_H
#define TEST_KVTREE_UTIL_H

#include "test_kvtree.h"

int test_kvtree_util_set_get_bytecount();
int test_kvtree_util_set_get_int();
int test_kvtree_util_set_get_unsigned_long();
int test_kvtree_util_set_get_str();
int test_kvtree_util_set_get_int64();
int test_kvtree_util_set_get_double();

void test_kvtree_util_init();

#endif //TEST_KVTREE_UTIL_H
