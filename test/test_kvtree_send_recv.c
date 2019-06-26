#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "kvtree_mpi.h"
#include "mpi.h"
#include <stdio.h>


int main(int argc, char** argv){
  kvtree* recv;
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

  kvtree* kvtree = kvtree_new();
  recv = kvtree_new();
  if(rank == 0){
    //send to 2
    kvtree_util_set_int(kvtree, "ONE", 1);
    kvtree_util_set_int(kvtree, "TWO", 2);
    kvtree_rc = kvtree_send(kvtree, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 0 send to 2 failed");
      rc = TEST_FAIL;
    }

    //receive from 1
    kvtree_rc = kvtree_recv(recv, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 0 receive from 1 failed");
      rc = TEST_FAIL;
    }
    if (recv == NULL){
      printf("in rank 0 received hash is NULL");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 0 could not read received value of 'ONE'");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 0 could not read received value of 'TWO'");
      rc = TEST_FAIL;
    }
    if ((val_of_one != 11) || (val_of_two != 22)){
      printf("in rank 0 received values are not correct");
      rc = TEST_FAIL;
    }
  }
  if(rank == 1){
    //receive from 2
    kvtree_rc = kvtree_recv(recv, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 1 receive from 2 failed");
      rc = TEST_FAIL;
    }
    if (recv == NULL){
      printf("in rank 1 received hash is NULL");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 1 could not read received value of 'ONE'");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 1 could not read received value of 'TWO'");
      rc = TEST_FAIL;
    }
    if ((val_of_one != 111) || (val_of_two != 222)){
      printf("in rank 1 received values are not correct");
      rc = TEST_FAIL;
    }

    //send to 0
    kvtree_util_set_int(kvtree, "ONE", 11);
    kvtree_util_set_int(kvtree, "TWO", 22);
    kvtree_rc = kvtree_send(kvtree, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 1 send to 0 failed");
      rc = TEST_FAIL;
    }
  }
  if(rank == 2){
    //receive from 0
    kvtree_rc = kvtree_recv(recv, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 2 receive from 0 failed");
      rc = TEST_FAIL;
    }
    if (recv == NULL){
      printf("in rank 2 received hash is NULL");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 2 could not read received value of 'ONE'");
      rc = TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 2 could not read received value of 'TWO'");
      rc = TEST_FAIL;
    }
    if ((val_of_one != 1) || (val_of_two != 2)){
      printf("in rank 2 received values are not correct");
      rc = TEST_FAIL;
    }

    //send to 1
    kvtree_util_set_int(kvtree, "ONE", 111);
    kvtree_util_set_int(kvtree, "TWO", 222);
    kvtree_rc = kvtree_send(kvtree, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("in rank 2 send to 1 failed");
      rc = TEST_FAIL;
    }
  }
printf("Rank = %d; ONE = %d; TWO = %d; rc = %d\n", rank, val_of_one, val_of_two, rc);
  kvtree_delete(&kvtree);
  if (kvtree != NULL){
    rc = TEST_FAIL;
    printf("deletion of kvtree failed");
  }
  kvtree_delete(&recv);
  if (recv != NULL){
    rc = TEST_FAIL;
    printf("deletion of recv failed");
  }

  MPI_Finalize();
  return rc;
}
