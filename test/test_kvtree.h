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
