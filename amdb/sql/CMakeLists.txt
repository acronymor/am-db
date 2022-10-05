CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

LIST(APPEND amdb_sql_folder storage)

FOREACH (folder ${amdb_sql_folder})
  FILE(GLOB srcs ${folder}/*.cc)
  LIST(FILTER srcs EXCLUDE REGEX ${folder}/.*test.*)
  LIST(APPEND amdb_sql_src ${srcs})

  FILE(GLOB test_srcs ${folder}/*test.cc)
  LIST(APPEND amdb_sql_test_src ${test_srcs})

  FILE(GLOB testutil_srcs ${folder}/*testutil.cc)
  LIST(APPEND amdb_sql_testutil_src ${testutil_srcs})
ENDFOREACH ()

ADD_LIBRARY(libamdb_sql OBJECT ${amdb_sql_src})
TARGET_LINK_LIBRARIES(libamdb_sql gflags glog)

ADD_EXECUTABLE(amdb main.h main.cc)
TARGET_LINK_LIBRARIES(amdb libamdb_common libamdb_sql)