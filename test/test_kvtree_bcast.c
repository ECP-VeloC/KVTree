#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "kvtree_mpi.h"
#include "mpi.h"
#include <stdio.h>


int non_empty_broadcast_test(kvtree* kvtree_1, kvtree* kvtree_2, int rank, const char*one, int val_of_one, const char* two, int val_of_two, const char* three, int val_of_three){
//  int rc = TEST_PASS;
  int kvtree_rc;
  //points to the hash that's passed to the kvtree_bcast function.
  //we want the receiving hash for rank 1 to be kvtree_1. For other processes,
  //sending/receiving hash should be kvtree
  kvtree *kvtree_sent_received;
  kvtree_sent_received = ( rank == 1? kvtree_2 : kvtree_1 );
  kvtree_rc = kvtree_bcast( kvtree_sent_received , 0, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("kvtree_bcast non-empty operation failed");
    return TEST_FAIL;
  }
  if ( kvtree_sent_received == NULL){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, kvtree_bcast operation resulted in NULL kvtree\n", rank);
    return TEST_FAIL;
  }
  kvtree_rc = kvtree_util_get_int(kvtree_sent_received , one, &val_of_one);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, kvtree_sent_received  read of 'ONE' failed\n", rank);
    return TEST_FAIL;
  }
  kvtree_rc = kvtree_util_get_int(kvtree_sent_received , two, &val_of_two);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, kvtree_sent_received  read of 'TWO' failed\n", rank);
    return TEST_FAIL;
  }
  if((val_of_one != 1) || (val_of_two != 2)){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, kvtree_sent_received  read of 'ONE' OR 'TWO' returned wrong value\n", rank);
    return TEST_FAIL;
  }
  kvtree_rc = kvtree_util_get_int(kvtree_sent_received , three, &val_of_three);
  if (kvtree_rc == KVTREE_SUCCESS){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, kvtree_sent_received  read of nonexisting key 'THREE' succeeded\n", rank);
    return TEST_FAIL;
  }
printf("Rank = %d; ONE = %d; TWO = %d\n", rank, val_of_one, val_of_two);
  //check that for process 1 kvtree_1 hash is unaffected by bcast
  if(rank == 1){
    kvtree_rc = kvtree_util_get_int(kvtree_1, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'ONE' failed\n");
      return TEST_FAIL;
    }
    else if((val_of_one != -1)){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'ONE' returned wrong value\n");
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(kvtree_1, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'TWO' failed\n");
      return TEST_FAIL;
    }
    else if((val_of_two != -2)){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'TWO' returned wrong value\n");
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(kvtree_1, three, &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'THREE' failed\n");
      return TEST_FAIL;
    }
    else if((val_of_three != -3)){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      printf("rank = 1, kvtree_1 read of 'THREE' returned wrong value\n");
      return TEST_FAIL;
    }
  }
  return TEST_PASS;
}

int empty_broadcast_test(kvtree *kvtree_1, int rank){
  int rc = TEST_PASS;
  int kvtree_rc = TEST_PASS;
  //re-initialize kvtree to empty n rank 0 and broadcast
  if(rank==0) kvtree_1 = kvtree_new();
  kvtree_rc = kvtree_bcast(kvtree_1, 0, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("kvtree_bcast empty operation failed");
    return TEST_FAIL;
  }
  if(kvtree_size(kvtree_1) != 0)
  {
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, broadcast of empty tree resulted in non-empty tree on receprtion\n", rank);
    return TEST_FAIL;
  }

  return TEST_PASS;
}

int main(int argc, char** argv){
  kvtree* kvtree_1;
  kvtree* kvtree_2;
  //this variable will hold the pointer to sending hash in sending process and
  //receving hash in receiving process.
 // int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[] = "ONE", two[] = "TWO", three[] = "THREE";
  kvtree* kvtree;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  if( ranks != 3){
    printf("tests require 3 processes; actual # is %d\n", ranks);
    return TEST_FAIL;
  }

  kvtree_1 = kvtree_new();
  kvtree_2 = kvtree_new();
  if(rank == 0){
    kvtree_util_set_int(kvtree_1, "ONE", 1);
    kvtree_util_set_int(kvtree_1, "TWO", 2);
  }
  if(rank == 1){
    kvtree_util_set_int(kvtree_1, "ONE", -1);
    kvtree_util_set_int(kvtree_1, "TWO", -2);
    kvtree_util_set_int(kvtree_1, "THREE", -3);
    kvtree_util_set_int(kvtree_2, "ONE", -1);
    kvtree_util_set_int(kvtree_2, "TWO", -2);
    kvtree_util_set_int(kvtree_2, "THREE", 3);
  }
  if(rank == 2){
    kvtree_util_set_int(kvtree_1, "ONE", -1);
    kvtree_util_set_int(kvtree_1, "THREE", 3);
  }
  kvtree_rc = non_empty_broadcast_test(kvtree_1, kvtree_2, rank, one, val_of_one, two, val_of_two, three, val_of_three);
  if(kvtree_rc != TEST_PASS){
    return TEST_FAIL;
  }
  kvtree_rc = empty_broadcast_test(kvtree_1, rank);
  if(kvtree_rc != TEST_PASS){
    return TEST_FAIL;
  }
  kvtree_delete(&kvtree_1);
  if (kvtree_1 != NULL){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, deletion of kvtree_1 failed\n", rank);
    return TEST_FAIL;
  }
  kvtree_delete(&kvtree_2);
  if (kvtree_2 != NULL){
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    printf("rank =%d, deletion of kvtree_2 failed\n", rank);
    return TEST_FAIL;
  }

  MPI_Finalize();
  return TEST_PASS;
}
