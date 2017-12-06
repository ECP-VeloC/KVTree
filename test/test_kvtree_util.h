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
