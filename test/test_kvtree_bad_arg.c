#include "kvtree.h"
#include "test_kvtree.h"
#include "test_kvtree_util.h"
#include "kvtree_mpi.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv){
  int rank, ranks;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranks);
  if( ranks != 3){
    printf("tests require 3 processes; actual # is %d\n", ranks);
    return TEST_FAIL;
  }

//  kvtree_rc = kvtree_bcast(NULL, 0, MPI_COMM_WORLD);
//  kvtree_rc = kvtree_send(NULL, 0, MPI_COMM_WORLD);
//  kvtree_rc = kvtree_recv(NULL, 0, MPI_COMM_WORLD);
//  kvtree_rc = kvtree_exchange_sendq(NULL, 0, NULL);
//  kvtree_rc = kvtree_exchange(NULL, NULL, MPI_COMM_WORLD);
//  kvtree_rc = kvtree_write_gather(NULL, NULL, MPI_COMM_WORLD);
//  kvtree_rc = kvtree_sendrecv (NULL, (rank+1) % ranks, NULL, (rank-1+ranks) % ranks, MPI_COMM_WORLD);
//kvtree_read_scatter should not succeed with null inputs
  if(kvtree_read_scatter(NULL, NULL, MPI_COMM_WORLD) == KVTREE_SUCCESS){
    return TEST_FAIL;
  }

  MPI_Finalize();
  return TEST_PASS;
}
