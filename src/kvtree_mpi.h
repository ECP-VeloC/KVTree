/*
* Copyright (c) 2018, Lawrence Livermore National Security, LLC.
* Produced at the Lawrence Livermore National Laboratory.
*
* Copyright (c) 2018, UChicago Argonne LLC,
*   operator of Argonne National Laboratory
*
* LLNL-CODE-745961
* All rights reserved.
*
* This is the license for KVTree.
* For details, see https://github.com/LLNL/KVTree
* Please read the LICENSE file for full license text.
*/

#ifndef KVTREE_MPI_H
#define KVTREE_MPI_H

#include "mpi.h"
#include "kvtree.h"

/*
=========================================
Hash MPI transfer functions
=========================================
*/

/* packs and send the given hash to the specified rank */
int kvtree_send(const kvtree* hash, int rank, MPI_Comm comm);

/* receives a hash from the specified rank and unpacks it into specified hash */
int kvtree_recv(kvtree* hash, int rank, MPI_Comm comm);

/* send and receive a hash in the same step */
int kvtree_sendrecv(
    const kvtree* hash_send, int rank_send,
          kvtree* hash_recv, int rank_recv,
    MPI_Comm comm
);

/* broadcasts a hash from a root and unpacks it into specified hash on all other tasks */
int kvtree_bcast(kvtree* hash, int root, MPI_Comm comm);

/* execute a (sparse) global exchange, similar to an alltoallv operation
 *
 * hash_send specifies destinations as:
 * <rank_X>
 *   <hash_to_send_to_rank_X>
 * <rank_Y>
 *   <hash_to_send_to_rank_Y>
 *
 * hash_recv returns hashes sent from remote ranks as:
 * <rank_A>
 *   <hash_received_from_rank_A>
 * <rank_B>
 *   <hash_received_from_rank_B> */
int kvtree_exchange(const kvtree* hash_send, kvtree* hash_recv, MPI_Comm comm);

typedef enum {
  KVTREE_EXCHANGE_RIGHT = 0,
  KVTREE_EXCHANGE_LEFT,
} kvtree_exchange_enum;

/* like kvtree_exchange, but with a direction specified for Bruck's
 * algorithm */
int kvtree_exchange_direction(
  const kvtree* hash_send,
        kvtree* hash_recv,
  MPI_Comm comm,
  kvtree_exchange_enum direction
);

#endif
