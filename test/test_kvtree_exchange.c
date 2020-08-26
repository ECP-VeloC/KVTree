#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "kvtree_mpi.h"
#include "stdio.h"

//this test queues up send hash in each of the processes using kvtree_exchange_sendq
//and sends to a process specified in the kvtree_exchange_sendq function using kvtree_exchange.
//In the send hash, the structure that is sent is found under the key that corresponds to the string value of the rank that the structure was sent to.
//In the recv hash, structure that was received is found under the key that corresponds to the string value of the rank that the structure was received from.
int exchange(int dir, int rank, int ranks){
  //int return TEST_PASS;
  int kvtree_rc;
  int val_of_three;
  kvtree* msg;
  msg  = kvtree_new();
  kvtree* recv;
  recv  = kvtree_new();
  kvtree* send;
  send = kvtree_new();
  kvtree* treeLeaf;

printf("rank=%d\n",rank);
  if(rank == 2){
    kvtree_util_set_int(msg, "THREEE", 3);
    kvtree_rc = kvtree_exchange_sendq(send, 0, msg);
    if (kvtree_rc != KVTREE_SUCCESS){
      return TEST_FAIL;
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    }
  }
  if(rank == 0){
    kvtree_util_set_int(msg, "THREEE", 33);
    kvtree_rc = kvtree_exchange_sendq(send, 1, msg);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  if(rank == 1){
    kvtree_util_set_int(msg, "THREEE", 333);
    kvtree_rc = kvtree_exchange_sendq(send, 2, msg);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  printf("SEND HASH BEFORE SENDING\n");
  kvtree_print(send, 4); 
  printf("RECEIVE HASH BEFORE SENDING\n");
  kvtree_print(recv, 4);
  if(dir == -1) 
    kvtree_rc = kvtree_exchange_direction(send, recv, MPI_COMM_WORLD, KVTREE_EXCHANGE_LEFT);
  else if(dir == 1)
    kvtree_rc = kvtree_exchange_direction(send, recv, MPI_COMM_WORLD, KVTREE_EXCHANGE_RIGHT);
  else if(dir == 0)
    kvtree_rc = kvtree_exchange(send, recv, MPI_COMM_WORLD);
  printf("SEND HASH AFTER SENDING\n");
  kvtree_print(send, 4); 
  printf("RECEIVE HASH AFTER SENDING\n");
  kvtree_print(recv, 4); 
  if(rank == 1){
//kvtree_elem* elem;
//elem = kvtree_elem_first(kvtree);
//char* key = kvtree_elem_key(elem);
//printf("key=%s\n", key);
    treeLeaf =  kvtree_get(send, "2");
    //kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, send read failed in rank 1\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 333){
      printf("test_kvtree_exchange, send read in rank 1 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    treeLeaf =  kvtree_get(recv, "0");
    //kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, recv read failed in rank 1\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 33){
      printf("test_kvtree_exchange, recv read in rank 1 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  else if(rank == 0){
    treeLeaf =  kvtree_get(send, "1");
    //kvtree_print(treeLeaf, 4);
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, send read failed in rank 0\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 33){
      printf("test_kvtree_exchange, send read in rank 0 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    treeLeaf =  kvtree_get(recv, "2");
    //kvtree_print(treeLeaf, 4);
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, recv read failed in rank 0\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 3){
      printf("test_kvtree_exchange, recv read in rank 0 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  else if(rank == 2){
    treeLeaf =  kvtree_get(send, "0");
    //kvtree_print(treeLeaf, 4);
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, send read failed in rank 2\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 3){
      printf("test_kvtree_exchange, send read in rank 2 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    treeLeaf =  kvtree_get(recv, "1");
    //kvtree_print(treeLeaf, 4);
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_exchange, recv read failed in rank 2\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 333){
      printf("test_kvtree_exchange, recv read in rank 2 returned wrong value\n");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }

  kvtree_delete(&send);
  if (send != NULL){
    printf("test_kvtree_exchange, deletion of send failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  kvtree_delete(&recv);
  if (recv != NULL){
    printf("test_kvtree_exchange, deletion of recv failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }

  return TEST_PASS;
}
int main(int argc, char** argv){
  int rank,ranks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  if( ranks != 3){
    printf("test_kvtree_exchange, tests require 3 processes; actual # is %d\n", ranks);
    return TEST_FAIL;
  }
  //call sraight kvtree_exchange
  if(exchange(0, rank, ranks) == TEST_FAIL) return TEST_FAIL;
  //call kvtree_exchange_direction RIGHT
  if(exchange(1, rank, ranks) == TEST_FAIL) return TEST_FAIL;
  //call kvtree_exchange_direction LEFT
  if(exchange(-1, rank, ranks) == TEST_FAIL) return TEST_FAIL;

  MPI_Finalize();
  return TEST_PASS;
}
