#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv){
  int rc = TEST_PASS;
  int rank, ranks, kvtree_rc;
  int val_of_three;
  kvtree* msg;
  msg  = kvtree_new();
  kvtree* recv;
  recv  = kvtree_new();
  kvtree* treeLeaf;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);

  kvtree* kvtree = kvtree_new();

  if(rank == 2){
    kvtree_util_set_int(msg, "THREEE", 3);
    kvtree_rc = kvtree_exchange_sendq(kvtree, 2, msg);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  if(rank == 0){
    kvtree_util_set_int(msg, "THREEE", 33);
    kvtree_rc = kvtree_exchange_sendq(kvtree, 1, msg);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  if(rank == 1){
    kvtree_util_set_int(msg, "THREEE", 333);
    kvtree_rc = kvtree_exchange_sendq(kvtree, 0, msg);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
  }
  kvtree_rc = kvtree_exchange(kvtree, recv, MPI_COMM_WORLD);
  if(rank == 1){
    kvtree_print(kvtree, 4); 
    kvtree_print(recv, 4); 
//kvtree_elem* elem;
//elem = kvtree_elem_first(kvtree);
//char* key = kvtree_elem_key(elem);
//printf("key=%s\n", key);
    treeLeaf =  kvtree_get(kvtree, "0");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 333) rc = TEST_FAIL;
    treeLeaf =  kvtree_get(recv, "0");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 33) rc = TEST_FAIL;
printf("rc=%d\n",rc);
  }
  else if(rank == 0){
    kvtree_print(kvtree, 4); 
    kvtree_print(recv, 4); 
    treeLeaf =  kvtree_get(kvtree, "1");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 33) rc = TEST_FAIL;
    treeLeaf =  kvtree_get(recv, "1");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 333) rc = TEST_FAIL;
printf("rc=%d\n",rc);
  }
  else if(rank == 2){
    kvtree_print(kvtree, 4); 
    kvtree_print(recv, 4); 
    treeLeaf =  kvtree_get(kvtree, "2");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 3) rc = TEST_FAIL;
    treeLeaf =  kvtree_get(recv, "2");
    kvtree_print(treeLeaf, 4); 
    kvtree_rc = kvtree_util_get_int(treeLeaf, "THREEE", &val_of_three);
    if (kvtree_rc != KVTREE_SUCCESS) rc = TEST_FAIL;
    if(val_of_three != 3) rc = TEST_FAIL;
printf("rc=%d\n",rc);
  }

  kvtree_delete(&kvtree);
  if (kvtree != NULL) rc = TEST_FAIL;
  MPI_Finalize();
  return rc;
}
