###############
# Build tests
###############

LIST(APPEND kvtree_test_srcs
    test_kvtree.c
    test_kvtree_allocate_delete.c
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
