#include "test_kvtree.h"
#include "test_kvtree_allocate_delete.h"
#include "test_kvtree_kv.h"
#include "test_kvtree_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void register_test(test_ptr_t test, char* test_name){
  char* name = strdup(test_name);
  test_ptrs[num_tests] = test;
  test_names[num_tests] = name;
  num_tests++;
}

int main(int argc, char** argv){
  /* Initialize state to be filled in by init functions */
  num_tests = 0;
  test_ptrs = malloc(MAX_TESTS * sizeof(test_ptr_t));
  test_names = malloc(MAX_TESTS * sizeof(char*));

  /* Initialize each test type */
  test_kvtree_allocate_delete_init();
  test_kvtree_kv_init();
  test_kvtree_util_init();
  //test_kvtree_bcast_init();

  /* Initialize state to count failures */
  int num_failed = 0;
  char** tests_failed = malloc(num_tests * sizeof(char*));

  /* Run all tests */
  int i;
  for(i = 0; i < num_tests; i++){
    if(test_ptrs[i]() != TEST_PASS){
      tests_failed[num_failed] = test_names[i];
      num_failed++;
    }
  }

  /* Print results */
  printf("Ran %d tests: %d pass, %d fail.\n", num_tests, num_tests - num_failed, num_failed);
  for(i = 0; i < num_failed; i++){
    printf("    %s FAILED.\n", tests_failed[i]);
  }

  /* Free all allocated memory */
  for(i = 0; i < num_tests; i++){
    free(test_names[i]);
  }
  free(test_ptrs);
  free(test_names);
  free(tests_failed);

  /* Return number of tests failed as return code */
  return num_failed;
}
