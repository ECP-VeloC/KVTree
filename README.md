# Key-Value Tree (KVTree) Library

The Key-Value Tree (KVTree) library provides a fully extensible C data
structure modeled after Perl hashes.

KVTree was originally part of the SCR Library. For now, documentation
can be found within the SCR library.

## Quickstart

KVTree uses the CMake build system and we recommend out-of-source builds.

```shell
git clone git@github.com:llnl/kvtree.git
mkdir build
mkdir install

cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ../kvtree
make
make install
make test
```

Some useful CMake command line options:

- `-DCMAKE_INSTALL_PREFIX=[path]`: Place to install the KVTree library
- `-DCMAKE_BUILD_TYPE=[Debug/Release]`: Build with debugging or optimizations
- `-DMPI`: Build with support for MPI movement of kvtree objects

### Dependencies

- C
- MPI (optional)
- CMake, Version 2.8+

## Authors

KVTree is part of the SCR project

Numerous people have [contributed](https://github.com/llnl/scr/graphs/contributors) to the SCR project.

To reference SCR in a publication, please cite the following paper:

* Adam Moody, Greg Bronevetsky, Kathryn Mohror, Bronis R. de Supinski, [Design, Modeling, and Evaluation of a Scalable Multi-level Checkpointing System](http://dl.acm.org/citation.cfm?id=1884666), LLNL-CONF-427742, Supercomputing 2010, New Orleans, LA, November 2010.

Additional information and research publications can be found here:

[http://computation.llnl.gov/projects/scalable-checkpoint-restart-for-mpi](http://computation.llnl.gov/projects/scalable-checkpoint-restart-for-mpi)
