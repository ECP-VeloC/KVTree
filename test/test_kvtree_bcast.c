#include "test_kvtree.h"
#include "test_kvtree_bcast.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv){
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[3] = "ONE", two[3] = "TWO", three[5] = "THREE";
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);

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
  if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  if (kvtree == NULL) rc = TEST_FAIL;
  kvtree_rc = kvtree_util_get_int(kvtree, one, &val_of_one);
  if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  kvtree_rc = kvtree_util_get_int(kvtree, two, &val_of_two);
  if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  if((val_of_one != 1) || (val_of_two != 2)) rc = TEST_FAIL;
  kvtree_rc = kvtree_util_get_int(kvtree, three, &val_of_three);
  if (kvtree_rc = KVTREE_SUCCESS) rc = TEST_FAIL;
printf("Rank = %d; ONE = %d; TWO = %d; rc = %d\n", rank, val_of_one, val_of_two, rc);
  kvtree_delete(&kvtree);
  if (kvtree != NULL) rc = TEST_FAIL;

  MPI_Finalize();
  return rc;
}
