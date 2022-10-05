CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

FILE(GLOB_RECURSE amdb_common_src "*.cc")
FILE(GLOB_RECURSE amdb_common_test_src "*_test.cc")
FOREACH (test_file ${amdb_common_test_src})
  LIST(REMOVE_ITEM amdb_common_src ${test_file})
ENDFOREACH ()

ADD_LIBRARY(libamdb_common ${amdb_common_src})
TARGET_LINK_LIBRARIES(libamdb_common glog gflags fmt)
