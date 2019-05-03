#include "test_kvtree.h"
#include "test_kvtree_send_recv.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv){
  kvtree* rec;
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_one, val_of_two, val_of_three;
  const char one[3] = "ONE", two[3] = "TWO", three[5] = "THREE";
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);

  kvtree* kvtree = kvtree_new();
  rec = kvtree_new();
  if(rank == 0){
    kvtree_util_set_int(kvtree, "ONE", 1);
    kvtree_util_set_int(kvtree, "TWO", 2);
    kvtree_rc = kvtree_send(kvtree, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  if(rank == 1){
    kvtree_util_set_int(kvtree, "ONE", 11);
    kvtree_util_set_int(kvtree, "TWO", 22);
    kvtree_rc = kvtree_send(kvtree, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  if(rank == 2){
    kvtree_util_set_int(kvtree, "ONE", 111);
    kvtree_util_set_int(kvtree, "TWO", 222);
    kvtree_rc = kvtree_send(kvtree, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  if(rank == 0){
    kvtree_rc = kvtree_recv(rec, 1, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if (rec == NULL) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if((val_of_one != 11) || (val_of_two != 22)) rc = TEST_FAIL;
  }
  if(rank == 1){
    kvtree_rc = kvtree_recv(rec, 2, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if (rec == NULL) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if((val_of_one != 111) || (val_of_two != 222)) rc = TEST_FAIL;
  }
  if(rank == 2){
    kvtree_rc = kvtree_recv(rec, 0, MPI_COMM_WORLD);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if (rec == NULL) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, one, &val_of_one);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    kvtree_rc = kvtree_util_get_int(rec, two, &val_of_two);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if((val_of_one != 1) || (val_of_two != 2)) rc = TEST_FAIL;
  }
printf("Rank = %d; ONE = %d; TWO = %d; rc = %d\n", rank, val_of_one, val_of_two, rc);
  kvtree_delete(&kvtree);
  if (kvtree != NULL) rc = TEST_FAIL;
  
  MPI_Finalize();
  return rc;
}

