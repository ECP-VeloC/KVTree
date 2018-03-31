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

# Header files need not be enumerated
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR})

###########
# LIB KVTREE #
###########

# Install header files
LIST(APPEND libkvtree_install_headers
	kvtree.h
    kvtree_util.h
)
IF(MPI)
    LIST(APPEND libkvtree_install_headers kvtree_mpi.h)
ENDIF(MPI)
INSTALL(FILES ${libkvtree_install_headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

LIST(APPEND libkvtree_srcs
	kvtree.c
    kvtree_util.c
    kvtree_io.c
    kvtree_helpers.c
    kvtree_err.c
    tv_data_display.c
)

IF(MPI)
	LIST(APPEND libkvtree_srcs kvtree_mpi.c kvtree_io_mpi.c)
ENDIF(MPI)

# KVTREE Library
ADD_LIBRARY(kvtree_o OBJECT ${libkvtree_srcs})

ADD_LIBRARY(kvtree SHARED $<TARGET_OBJECTS:kvtree_o>)
TARGET_LINK_LIBRARIES(kvtree ${KVTREE_EXTERNAL_LIBS})
SET_TARGET_PROPERTIES(kvtree PROPERTIES OUTPUT_NAME kvtree CLEAN_DIRECT_OUTPUT 1)
INSTALL(TARGETS kvtree DESTINATION ${CMAKE_INSTALL_LIBDIR})

ADD_LIBRARY(kvtree-static STATIC $<TARGET_OBJECTS:kvtree_o>)
IF(KVTREE_LINK_STATIC)
        SET_TARGET_PROPERTIES(kvtree-static PROPERTIES LINK_SEARCH_START_STATIC 1)
        SET_TARGET_PROPERTIES(kvtree-static PROPERTIES LINK_SEARCH_END_STATIC 1)
ENDIF(KVTREE_LINK_STATIC)
TARGET_LINK_LIBRARIES(kvtree-static ${KVTREE_EXTERNAL_LIBS})

SET_TARGET_PROPERTIES(kvtree-static PROPERTIES OUTPUT_NAME kvtree CLEAN_DIRECT_OUTPUT 1)
INSTALL(TARGETS kvtree-static DESTINATION ${CMAKE_INSTALL_LIBDIR})


###########
# CLI KVTREE #
###########

# Individual binaries generated from a single .c file
LIST(APPEND clikvtree_c_bins
    kvtree_print_file
)

# Build and install C binaries
FOREACH(bin IN ITEMS ${clikvtree_c_bins})
	ADD_EXECUTABLE(${bin} ${bin}.c)
	TARGET_LINK_LIBRARIES(${bin} kvtree)
	INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${bin} DESTINATION ${CMAKE_INSTALL_BINDIR})
ENDFOREACH(bin IN ITEMS ${clikvtree_c_bins})
