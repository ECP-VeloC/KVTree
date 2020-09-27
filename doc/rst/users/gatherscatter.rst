kvtree gather/scatter
+++++++++++++++++++++
The kvtree library contains a function called kvtree_write_gather() to gather kvtrees from a set of MPI ranks to be stored to a set of files.
The data from kvtrees from multiple ranks are merged and stored
in a set of files that is more efficient than having each rank store its kvtree to its own file.
Similarly, there is a read function called kvtree_read_scatter() to scatter the kvtrees back to a set of MPI ranks.
These functions are implemented in kvtree_mpi_io.c. 

For large jobs, a single merged kvtree may consist of more bytes than can be loaded into any single MPI process.
This information is scattered among multiple files that are organized as a tree.
These files are written to a path and filename prefix as specified by the caller.
There is always a root file.
Here are the contents of an example root::

  LEVEL
    1
  RANKS
    4
  RANK
    0
      OFFSET
        0
      FILE
        .0.0

The LEVEL field lists the level at which the current file is located in the tree.
The leaves of the tree are at level 0.
The RANKS field specifies the number of ranks the current file
(and its associated subtree) contains information for.

For levels that are higher than level 0, the RANK kvtree
contains information about other files to be read.
Each entry in this kvtree is identified by a rank id,
and then for each rank, a FILE and OFFSET are given.
The rank id specifies which MPI rank is responsible for reading content at the next level.
The FILE field specifies the suffix of the file name that is to be read,
and the OFFSET field gives the starting byte offset within that file.

A process reading a file at the current level scatters the kvtree info
to the designated "reader" ranks, and those processes read data for the next level.
In this way, the task of reading the data is distributed among multiple processes in the job.
The kvtree library ensures that the maximum amount of data any process reads in any step
is limited (currently 1MB).

File names at levels lower than the root have names of the form <prefix>.<level>.<rank>,
where prefix is provided by the caller, level is the level number within the tree,
and rank is the rank of the process that wrote the file.

Finally, level 0 contains the original kvtree data for a set of MPI ranks.
Here are the contents of an example file at level 0::

  LEVEL
    0
  RANKS
    4
  RANK
    0
      <kvtree_from_rank_0>
    1
      <kvtree_from_rank_1>
    2
      <kvtree_from_rank_2>
    3
      <kvtree_from_rank_3>

Again, the number of ranks that this file contains information for is recorded under the RANKS field.

There are entries for specific ranks under the RANK kvtree,
which is indexed by rank id within MPI_COMM_WORLD.
This kvtree corresponds to the original kvtree persisted by that particular rank during the call to kvtree_write_gather().
