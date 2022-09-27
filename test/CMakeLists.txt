###############
# Build tests
###############

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

LIST(APPEND kvtree_test_srcs
    test_kvtree.c
    test_kvtree_allocate_delete.c
    test_kvtree_kv.c
    test_kvtree_util.c
#    test_kvtree_bcast.c
)

IF(KVTREE_LINK_STATIC)
    SET(kvtree_lib kvtree-static)
ELSE()
    SET(kvtree_lib kvtree)
ENDIF()

ADD_EXECUTABLE(test_kvtree ${kvtree_test_srcs})
TARGET_LINK_LIBRARIES(test_kvtree ${kvtree_lib})

################
# Add tests to ctest
################

# rountdrip tests
ADD_TEST(roundtrip_tests test_kvtree)

# print test script
FIND_PROGRAM(PRINT_TEST ${CMAKE_CURRENT_SOURCE_DIR}/print_test)
ADD_TEST(NAME print_test COMMAND ${PRINT_TEST} ${PROJECT_BINARY_DIR}/src/kvtree_print ${CMAKE_CURRENT_SOURCE_DIR}/files)

# Test multiple processes writing to the same file at the same time
ADD_EXECUTABLE(test_kvtree_write_locking test_kvtree_write_locking.c)
TARGET_LINK_LIBRARIES(test_kvtree_write_locking PRIVATE ${kvtree_lib})
ADD_TEST(NAME test_kvtree_write_locking COMMAND test_kvtree_write_locking)

IF(MPI_FOUND)

CONFIGURE_FILE(kvtree_read_scatter_single_test.sh ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
ADD_EXECUTABLE(kvtree_read_scatter_single_test kvtree_read_scatter_single_test.c)
TARGET_LINK_LIBRARIES(kvtree_read_scatter_single_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_read_scatter_single_test.sh ${CMAKE_CURRENT_SOURCE_DIR}/files "")

ADD_EXECUTABLE(kvtree_bcast_test test_kvtree_bcast.c)
TARGET_LINK_LIBRARIES(kvtree_bcast_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_bcast_test 256 "")
#ADD_TEST(NAME kvtree_bcast_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_bcast_test)

ADD_EXECUTABLE(kvtree_send_recv_test test_kvtree_send_recv.c)
TARGET_LINK_LIBRARIES(kvtree_send_recv_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_send_recv_test 256 "")
#ADD_TEST(NAME kvtree_send_recv_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_send_recv_test)

ADD_EXECUTABLE(kvtree_exchange_test test_kvtree_exchange.c)
TARGET_LINK_LIBRARIES(kvtree_exchange_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_exchange_test 256 "")
#ADD_TEST(NAME kvtree_exchange_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_exchange_test)

ADD_EXECUTABLE(kvtree_write_gather_test test_kvtree_write_gather.c)
TARGET_LINK_LIBRARIES(kvtree_write_gather_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_write_gather_test 256 "")
#ADD_TEST(NAME kvtree_write_gather_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_write_gather_test)

ADD_EXECUTABLE(kvtree_bad_arg_test test_kvtree_bad_arg.c)
TARGET_LINK_LIBRARIES(kvtree_bad_arg_test PRIVATE ${kvtree_lib})
KVTREE_ADD_TEST(kvtree_bad_arg_test 256 "")
#ADD_TEST(NAME kvtree_bad_arg_test COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 3 ./kvtree_bad_arg_test)

ENDIF()
####################
# make a verbose "test" target named "check"
####################

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
