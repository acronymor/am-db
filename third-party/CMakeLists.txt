CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

ADD_SUBDIRECTORY(abseil-cpp)

# https://gflags.github.io/gflags/
FIND_PACKAGE(gflags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIRS})

# https://github.com/google/glog#user-guide
FIND_PACKAGE(Glog REQUIRED)
INCLUDE_DIRECTORIES(${GLOG_INCLUDE_DIRS})

# https://google.github.io/googletest/quickstart-cmake.html
FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

# https://brpc.apache.org/zh/docs/
SET(BRPC_OUTPUT ${CMAKE_SOURCE_DIR}/incubator-brpc/output)
ADD_LIBRARY(libbrpc INTERFACE)
TARGET_INCLUDE_DIRECTORIES(libbrpc INTERFACE ${BRPC_OUTPUT}/include)
TARGET_LINK_LIBRARIES(libbrpc INTERFACE
    ${BRPC_OUTPUT}/lib/libbrpc.so
    gflags ssl crypto dl
)
