#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv){
  kvtree* recv;
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[3] = "ONE", two[3] = "TWO", three[5] = "THREE";
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);

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
  if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  if (kvtree == NULL) rc = TEST_FAIL;
  kvtree_rc = kvtree_read_scatter("PREV_", recv, MPI_COMM_WORLD);
  if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  if (kvtree == NULL) rc = TEST_FAIL;
  if(rank == 0){
    kvtree_print(kvtree, 4);
    kvtree_print(recv, 4);
    kvtree_rc = kvtree_util_get_int(kvtree, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_one != 1) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(kvtree, "TWO", &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_two != 2) rc = TEST_FAIL;
  }
  if(rank == 2){
    kvtree_print(kvtree, 4);
    kvtree_print(recv, 4);
    kvtree_rc = kvtree_util_get_int(kvtree, "ONE", &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_one != -1) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(kvtree, "THREE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 3) rc = TEST_FAIL;
  }
  kvtree_delete(&kvtree);
  if (kvtree != NULL) rc = TEST_FAIL;

  MPI_Finalize();
  return rc;
}
