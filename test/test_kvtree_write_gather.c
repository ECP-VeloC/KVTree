#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"

//This is the test of kvtree_write_gather/kvtree_read_scatter functionality. The former writes a hash from multiple processes to files with specified prefex,
//and the latter reads them from files.

int main(int argc, char** argv){
  kvtree* recv;
  kvtree* send;
  //int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  if( ranks != 3){
    printf("test_kvtree_write_gather, tests require 3 processes; actual # is %d\n", ranks);
    return TEST_FAIL;
  }

  send = kvtree_new();
  recv = kvtree_new();
  if(rank == 0){
    kvtree_util_set_int(send, "ONE", 1);
    kvtree_util_set_int(send, "TWO", 2);
  }
  if(rank == 2){
    kvtree_util_set_int(send, "ONE", -1);
    kvtree_util_set_int(send, "THREE", 3);
  }

  kvtree_rc = kvtree_write_gather("PREFIX", send, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf("test_kvtree_write_gather, kvtree_write_gather operation failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  kvtree_rc = kvtree_read_scatter("PREFIX", recv, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    printf("test_kvtree_write_gather, kvtree_read_scatter operation failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  if (recv == NULL){
    printf("test_kvtree_write_gather, kvtree_read_scatter operation returned NULL kvtree");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  printf("rank=%d, send\n", rank);
  kvtree_print(send, 4);
  printf("rank=%d, recv\n", rank);
  kvtree_print(recv, 4);
  if(rank == 0){
    kvtree_rc = kvtree_util_get_int(recv, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_write_gather, rank=0, recv read of 'ONE' failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_one != 1){
      printf("test_kvtree_write_gather, rank=0, recv read of 'ONE' returned wrong value");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, "TWO", &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_write_gather, rank=0, recv read of 'TWO' failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_two != 2){
      printf("test_kvtree_write_gather, rank=0, recv read of 'TWO' returned wrong value");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  if(rank == 2){
    kvtree_rc = kvtree_util_get_int(recv, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_write_gather, rank=2, recv read of 'ONE' failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_one != -1){
      printf("test_kvtree_write_gather, rank=2, recv read of 'ONE' returned wrong value");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    kvtree_rc = kvtree_util_get_int(recv, "THREE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      printf("test_kvtree_write_gather, rank=2, recv read of 'THREE' failed");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
    if(val_of_three != 3){
      printf("test_kvtree_write_gather, rank=2, recv read of 'THREE' returned wrong value");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  if(rank ==1){
    if(kvtree_size(recv) != 0){
      printf("test_kvtree_write_gather, rank=1, sending hash of size 0 resulted in non-zero receive");
      printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
      return TEST_FAIL;
    }
  }
  kvtree_delete(&send);
  if (send != NULL){
    printf("test_kvtree_write_gather, deletion of kvtree failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }
  kvtree_delete(&recv);
  if (recv != NULL){
    printf("deletion of recv failed");
    printf ("Error in line %d, file %s, function %s.\n", __LINE__, __FILE__, __func__);
    return TEST_FAIL;
  }

  MPI_Finalize();
  return TEST_PASS;
}
