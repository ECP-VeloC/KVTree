#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "kvtree_mpi.h"
#include "mpi.h"
#include <stdio.h>


int main(int argc, char** argv){
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[] = "ONE", two[] = "TWO", three[] = "THREE";
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  
  if( ranks != 3){
    printf("tests require 3 processes; actual # is %d\n", ranks);
    return 1;
  }

  // initialize data
  kvtree* kvtree = kvtree_new();
  if(rank == 0){
    kvtree_util_set_int(kvtree, "ONE", 1);
    kvtree_util_set_int(kvtree, "TWO", 2);
  }
  if(rank == 2){
    kvtree_util_set_int(kvtree, "ONE", -1);
    kvtree_util_set_int(kvtree, "THREE", 3);
  }
  
  kvtree_rc = kvtree_bcast(kvtree, 0, MPI_COMM_WORLD);
  
  if (kvtree_rc != KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree_bcast operation failed");
  }
  
  if (kvtree == NULL){
    rc = TEST_FAIL;
    printf("kvtree_bcast operation resulted in NULL kvtree");
  }
  
  // all ranks should have a key ONE
  kvtree_rc = kvtree_util_get_int(kvtree, one, &val_of_one);
  if (kvtree_rc != KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree read of 'ONE' failed");
  }
  
  // all ranks should have a key TWO
  kvtree_rc = kvtree_util_get_int(kvtree, two, &val_of_two);
  if (kvtree_rc != KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree read of 'TWO' failed");
  }
  
  // all ranks should have { ONE: 1, TWO; 2 }
  if((val_of_one != 1) || (val_of_two != 2)){
    rc = TEST_FAIL;
    printf("kvtree read of 'ONE' OR 'TWO' returned wrong value");
  }
  
  // no rank should have a THREE key
  // even rank 2's THREE key is overwritten
  kvtree_rc = kvtree_util_get_int(kvtree, three, &val_of_three);
  if (kvtree_rc == KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree read of nonexisting key 'THREE' succeeded");
  }
  
  printf("Rank = %d; ONE = %d; TWO = %d; rc = %d\n", rank, val_of_one, val_of_two, rc);
  
  // clean up
  kvtree_delete(&kvtree);
  if (kvtree != NULL){
    rc = TEST_FAIL;
    printf("deletion of kvtree failed");
  }

  MPI_Finalize();
  return rc;
}
