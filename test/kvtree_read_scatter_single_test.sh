#!/bin/bash

# We have to run kvtree_read_scatter_single_test from a shell script
# so that we can set KVTREE_ENTRIES_PER_FILE=1
KVTREE_ENTRIES_PER_FILE=1 ./kvtree_read_scatter_single_test "${@}"
