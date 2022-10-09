CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

ADD_SUBDIRECTORY(abseil-cpp)
ADD_SUBDIRECTORY(fmt)
ADD_SUBDIRECTORY(glog)

# https://gflags.github.io/gflags/
FIND_PACKAGE(gflags 2.2.2 REQUIRED)
INCLUDE_DIRECTORIES(${GFLAGS_INCLUDE_DIRS})

# https://github.com/google/glog#user-guide
# FIND_PACKAGE(Glog 0.5.0 REQUIRED)
# INCLUDE_DIRECTORIES(${GLOG_INCLUDE_DIRS})

# https://google.github.io/googletest/quickstart-cmake.html
FIND_PACKAGE(GTest 1.11.0 REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

# https://github.com/google/leveldb
FIND_PACKAGE(leveldb 1.23.0 REQUIRED)
INCLUDE_DIRECTORIES(${LEVELDB_INCLUDE_DIRS})

# https://github.com/protocolbuffers/protobuf
FIND_PACKAGE(Protobuf 3.12.4 REQUIRED)
INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})

# https://brpc.apache.org/zh/docs/
SET(BRPC_OUTPUT ${CMAKE_SOURCE_DIR}/incubator-brpc/output)
ADD_LIBRARY(libbrpc INTERFACE)
TARGET_INCLUDE_DIRECTORIES(libbrpc INTERFACE ${BRPC_OUTPUT}/include)
TARGET_LINK_LIBRARIES(libbrpc INTERFACE
    ${BRPC_OUTPUT}/lib/libbrpc.so
    gflags ssl crypto dl
)
