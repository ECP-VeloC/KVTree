#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv){
  kvtree* recv;
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
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
    kvtree_util_set_int(kvtree, "ONE", 1);
    kvtree_util_set_int(kvtree, "TWO", 2);
  }
  if(rank == 2){
    kvtree_util_set_int(kvtree, "ONE", -1);
    kvtree_util_set_int(kvtree, "THREE", 3);
  }

  kvtree_rc = kvtree_write_gather("PREV_", kvtree, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree_write_gather operation failed");
  }
  kvtree_rc = kvtree_read_scatter("PREV_", recv, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS){
    rc = TEST_FAIL;
    printf("kvtree_read_scatter operation failed");
  }
  if (recv == NULL){
    rc = TEST_FAIL;
    printf("kvtree_read_scatter operation returned NULL kvtree");
  }
  if(rank == 0){
    kvtree_print(kvtree, 4);
    kvtree_print(recv, 4);
    kvtree_rc = kvtree_util_get_int(recv, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      rc = TEST_FAIL;
      printf("recv read of 'ONE' failed");
    }
    if(val_of_one != 1){
      rc = TEST_FAIL;
      printf("recv read of 'ONE' returned wrong value");
    }
    kvtree_rc = kvtree_util_get_int(recv, "TWO", &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS){
      rc = TEST_FAIL;
      printf("recv read of 'TWO' failed");
    }
    if(val_of_two != 2){
      rc = TEST_FAIL;
      printf("recv read of 'TWO' returned wrong value");
    }
  }
  if(rank == 2){
    kvtree_print(kvtree, 4);
    kvtree_print(recv, 4);
    kvtree_rc = kvtree_util_get_int(recv, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS){
      rc = TEST_FAIL;
      printf("recv read of 'ONE' failed");
    }
    if(val_of_one != -1){
      rc = TEST_FAIL;
      printf("recv read of 'ONE' returned wrong value");
    }
    kvtree_rc = kvtree_util_get_int(recv, "THREE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS){
      rc = TEST_FAIL;
      printf("recv read of 'THREE' failed");
    }
    if(val_of_three != 3){
      rc = TEST_FAIL;
      printf("recv read of 'THREE' returned wrong value");
    }
  }
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
