# Copyright (c) 2018, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory.
#
# Copyright (c) 2018, UChicago Argonne LLC,
#   operator of Argonne National Laboratory
#
# LLNL-CODE-745961
# All rights reserved.
#
# This is the license for KVTree.
# For details, see https://github.com/LLNL/KVTree
# Please read the LICENSE file for full license text.

###############
# Build tests
###############

LIST(APPEND kvtree_test_srcs
    test_kvtree.c
    test_kvtree_allocate_delete.c
    test_kvtree_kv.c
    test_kvtree_util.c
)

ADD_EXECUTABLE(test_kvtree ${kvtree_test_srcs})
TARGET_LINK_LIBRARIES(test_kvtree kvtree)

################
# Add tests to ctest
################

# rountdrip tests
ADD_TEST(roundtrip_tests test_kvtree)

# print test script
FIND_PROGRAM(PRINT_TEST ${CMAKE_CURRENT_SOURCE_DIR}/print_test)
ADD_TEST(NAME print_test COMMAND ${PRINT_TEST} ${PROJECT_BINARY_DIR}/src/kvtree_print_file ${CMAKE_CURRENT_SOURCE_DIR}/files)

####################
# make a verbose "test" target named "check"
####################

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
