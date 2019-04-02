#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "kvtree_mpi.h"
#include "mpi.h"
#include "stdio.h"

//This is the test of kvtree_send/kvtree_recv functionality.
//In order to avoid deadlock, kvtree_send preceeds kvtree_recv in process 0 only.

int kvT_send_kvT_recv(int rank, int ranks){
  kvtree* recv;
  kvtree* send;
//  int rc = TEST_PASS;
  int kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[] = "ONE", two[] = "TWO", three[] = "THREE";

  send = kvtree_new();
  recv = kvtree_new();
  if(rank == 0){
    //send to 2
    kvtree_util_set_int(send, "ONE", 1);
    kvtree_util_set_int(send, "TWO", 2);
    kvtree_rc = kvtree_send(send, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 send to 2 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }

    //receive from 1
    kvtree_rc = kvtree_recv(recv, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 receive from 1 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if (recv == NULL){
      printf("test_kvtree_send_recv, in rank 0 received hash is NULL");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 could not read received value of 'ONE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 could not read received value of 'TWO'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if ((val_of_one != 11) || (val_of_two != 22)){
      printf("test_kvtree_send_recv, in rank 0 received values are not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  if(rank == 1){
    //receive from 2
    kvtree_rc = kvtree_recv(recv, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 1 receive from 2 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if (recv == NULL){
      printf("test_kvtree_send_recv, in rank 1 received hash is NULL");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 1 could not read received value of 'ONE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 1 could not read received value of 'TWO'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if ((val_of_one != 111) || (val_of_two != 222)){
      printf("test_kvtree_send_recv, in rank 1 received values are not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }

    //send to 0
    kvtree_util_set_int(send, "ONE", 11);
    kvtree_util_set_int(send, "TWO", 22);
    kvtree_rc = kvtree_send(send, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 1 send to 0 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  if(rank == 2){
    //receive from 0
    kvtree_rc = kvtree_recv(recv, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 receive from 0 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if (recv == NULL){
      printf("test_kvtree_send_recv, in rank 2 received hash is NULL");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 could not read received value of 'ONE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 could not read received value of 'TWO'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if ((val_of_one != 1) || (val_of_two != 2)){
      printf("test_kvtree_send_recv, in rank 2 received values are not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }

    //send to 1
    kvtree_util_set_int(send, "ONE", 111);
    kvtree_util_set_int(send, "TWO", 222);
    kvtree_rc = kvtree_send(send, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 send to 1 failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  return TEST_PASS;
}
int kvT_send_recv(int rank, int ranks){
  int rc, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  kvtree* send = kvtree_new();
  kvtree* recv = kvtree_new();
  if(rank == 0)
    kvtree_util_set_int(send, "ONE", 1);
  else if(rank == 1) 
    kvtree_util_set_int(send, "TWO", 2);
  else
    kvtree_util_set_int(send, "THREE", 3);
// with ranks in a circular queue, send to 1 rank forward, receive from 1 rank back
  kvtree_rc = kvtree_sendrecv(send, (rank+1) % ranks, recv, (rank-1+ranks) % ranks, MPI_COMM_WORLD);
  if(kvtree_rc != TEST_PASS){
    printf("test_kvtree_send_recv, kvtree_sendrecv call failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  if(rank == 0){
    kvtree_rc = kvtree_util_get_int(send, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 could not read send value of 'ONE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if (val_of_one != 1){
      printf("test_kvtree_send_recv, in rank 0 send value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, "THREE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 could not read recv value of 'THREE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if (val_of_three != 3){
      printf("test_kvtree_send_recv, in rank 0 recv value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
  }
  if(rank == 1){
    kvtree_rc = kvtree_util_get_int(send, "TWO", &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 1 could not read send value of 'TWO'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    if (val_of_two != 2){
      printf("test_kvtree_send_recv, in rank 0 send value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 0 could not read recv value of 'ONE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    if (val_of_one != 1){
      printf("test_kvtree_send_recv, in rank 0 recv value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
  }
  if(rank == 2){
    kvtree_rc = kvtree_util_get_int(send, "THREE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 could not read send value of 'THREE'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    if (val_of_three != 3){
      printf("test_kvtree_send_recv, in rank 2 send value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, "TWO", &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_send_recv, in rank 2 could not read recv value of 'TWO'");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
    if (val_of_two != 2){
      printf("test_kvtree_send_recv, in rank 0 recv value is not correct");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
       return TEST_FAIL;
    }
  }

  printf("rank = %d\n", rank);
  printf("SEND HASH\n");
  kvtree_print(send, 4);
  printf("RECEIVE HASH\n");
  kvtree_print(recv, 4);
 
  
  kvtree_delete(&send);
  if (send != NULL){
    printf("test_kvtree_send_recv, deletion of kvtree failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
     return TEST_FAIL;
  }
  kvtree_delete(&recv);
  if (recv != NULL){
    printf("test_kvtree_send_recv, deletion of recv failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
     return TEST_FAIL;
  }

  return TEST_PASS;
}
int main(int argc, char** argv){
  int rank, ranks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  if( ranks != 3){
    printf("test_kvtree_send_recv, tests require 3 processes; actual # is %d\n", ranks);
    return 1;
  }
  //uses kvtree_send and kvtree_recv
  if(kvT_send_kvT_recv(rank, ranks) == TEST_FAIL)
    return TEST_FAIL;
  //uses kvtree_send_recv
  if(kvT_send_recv(rank, ranks) == TEST_FAIL)
    return TEST_FAIL;
  MPI_Finalize();
  return 0;
}
