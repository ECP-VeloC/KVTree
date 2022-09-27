CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
PROJECT(KVTREE VERSION 1.3.0)

# Mac rpath Policy
IF(POLICY CMP0042)
    CMAKE_POLICY(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)
SET(CMAKE_MACOSX_RPATH ON)

# Linux rpath settings
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Configuration Options
OPTION(MPI "Enable MPI operations for kvtrees" ON)
MESSAGE(STATUS "MPI: ${MPI}")

OPTION(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)
MESSAGE(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")

if(BUILD_SHARED_LIBS)
    OPTION(KVTREE_LINK_STATIC "Default to static linking? (Needed for Cray)" OFF)
else()
    OPTION(KVTREE_LINK_STATIC "Default to static linking? (Needed for Cray)" ON)
endif()
MESSAGE(STATUS "KVTREE_LINK_STATIC: ${KVTREE_LINK_STATIC}")

OPTION(ENABLE_TESTS "Whether to build tests" ON)
MESSAGE(STATUS "ENABLE_TESTS: ${ENABLE_TESTS}")

SET(KVTREE_FILE_LOCK "FLOCK" CACHE STRING "Specify type of file locking to use (FLOCK FCNTL NONE)")
SET_PROPERTY(CACHE KVTREE_FILE_LOCK PROPERTY STRINGS FLOCK FCNTL NONE)
MESSAGE(STATUS "KVTREE_FILE_LOCK: ${KVTREE_FILE_LOCK}")

OPTION(TVDISPLAY "Whether to compile tv_data_display.c for debugging with TotalView C++View" OFF)
MESSAGE(STATUS "TVDISPLAY: ${TVDISPLAY}")

# Find Packages & Files

LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

## MPI
IF(MPI)
    INCLUDE(SetupMPI)
    IF(NOT MPI_C_FOUND)
        MESSAGE(FATAL_ERROR
            "Could not find MPI! "
            "Either add an MPI compiler to your path "
            "or force CMake to build using the correct compiler (`export CC=mpicc`). "
            "To disable MPI, set -DMPI=OFF")
    ENDIF(NOT MPI_C_FOUND)
ENDIF(MPI)

## ZLIB
FIND_PACKAGE(ZLIB REQUIRED)

## HEADERS
INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE(byteswap.h HAVE_BYTESWAP_H)

# lib/ bin/ include/ dirs
INCLUDE(GNUInstallDirs)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_LIBDIR})

# Subdirectories
ADD_SUBDIRECTORY(src)

SET(KVTREE_TEST_BASE "/dev/shm" CACHE PATH "Default base path for SCR cache directory")
IF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
  SET(KVTREE_TEST_BASE "/tmp")
ENDIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
MESSAGE("Cache base is: " ${KVTREE_TEST_BASE})

IF(ENABLE_TESTS)
    INCLUDE(CTest)
    INCLUDE(KVTREE_ADD_TEST)
    ADD_SUBDIRECTORY(test)
ENDIF(ENABLE_TESTS)

# Generate config.h with all our build #defs
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/config.h.in ${PROJECT_BINARY_DIR}/config.h)
SET_SOURCE_FILES_PROPERTIES(${PROJECT_BINARY_DIR}/config.h PROPERTIES GENERATED TRUE)

# Special Install Files
INSTALL(FILES README.md DESTINATION ${CMAKE_INSTALL_DATADIR}/kvtree)

# CMake configs

# Install the exports file
INSTALL(EXPORT kvtreeTargets FILE kvtreeTargets.cmake NAMESPACE kvtree:: DESTINATION share/kvtree/cmake)

# Export so we can use in-tree
EXPORT(EXPORT kvtreeTargets NAMESPACE kvtree:: FILE kvtreeTargets.cmake)

# Setup the package config
INCLUDE(CMakePackageConfigHelpers)
CONFIGURE_PACKAGE_CONFIG_FILE(cmake/kvtreeConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/kvtreeConfig.cmake INSTALL_DESTINATION share/kvtree/cmake)
WRITE_BASIC_PACKAGE_VERSION_FILE(${CMAKE_CURRENT_BINARY_DIR}/kvtreeConfigVersion.cmake COMPATIBILITY SameMajorVersion)

# Install package config
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/kvtreeConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/kvtreeConfigVersion.cmake DESTINATION share/kvtree/cmake)

# Package
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "KV Storage Library for Perl style hashes")
SET(CPACK_PACKAGE_VENDOR "Lawrence Livermore National Laboratory")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.TXT")
