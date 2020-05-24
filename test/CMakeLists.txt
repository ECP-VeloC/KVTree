###############
# Build tests
###############

LIST(APPEND kvtree_test_srcs
    test_kvtree.c
    test_kvtree_allocate_delete.c
    test_kvtree_kv.c
    test_kvtree_util.c
#    test_kvtree_bcast.c
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
ADD_TEST(NAME print_test COMMAND ${PRINT_TEST} ${PROJECT_BINARY_DIR}/src/kvtree_print ${CMAKE_CURRENT_SOURCE_DIR}/files)

IF(MPI_FOUND)
ADD_EXECUTABLE(kvtree_bcast_test test_kvtree_bcast.c)
TARGET_LINK_LIBRARIES(kvtree_bcast_test ${KVTREE_EXTERNAL_LIBS} kvtree)
ADD_TEST(NAME kvtree_bcast_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_bcast_test)

ADD_EXECUTABLE(kvtree_send_recv_test test_kvtree_send_recv.c)
TARGET_LINK_LIBRARIES(kvtree_send_recv_test ${KVTREE_EXTERNAL_LIBS} kvtree)
ADD_TEST(NAME kvtree_send_recv_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_send_recv_test)

ADD_EXECUTABLE(kvtree_exchange_test test_kvtree_exchange.c)
TARGET_LINK_LIBRARIES(kvtree_exchange_test ${KVTREE_EXTERNAL_LIBS} kvtree)
ADD_TEST(NAME kvtree_exchange_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_exchange_test)

ADD_EXECUTABLE(kvtree_write_gather_test test_kvtree_write_gather.c)
TARGET_LINK_LIBRARIES(kvtree_write_gather_test ${KVTREE_EXTERNAL_LIBS} kvtree)
ADD_TEST(NAME kvtree_write_gather_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_write_gather_test)
ENDIF(MPI_FOUND)


####################
# make a verbose "test" target named "check"
####################

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
