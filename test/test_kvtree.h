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

#ifndef TEST_KVTREE_H
#define TEST_KVTREE_H

#include "kvtree.h"
#include "kvtree_util.h"

#define TEST_PASS (0)
#define TEST_FAIL (1)

/* MAX_TESTS can be changed arbitrarily as long as
 * it exceeds the number of tests
 */
#define MAX_TESTS (1024)
#define MAX_NAME_LENGTH (1024) //overkill

typedef int (*test_ptr_t)( void );

int num_tests;
test_ptr_t* test_ptrs;
char** test_names;

void register_test(test_ptr_t test, char* test_name);

#endif //TEST_KVTREE_H
