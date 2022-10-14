CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

LIST(APPEND amdb_sql_folder common codec chunk storage)

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
TARGET_LINK_LIBRARIES(
    libamdb_sql libamdb_proto libamdb_common
    absl::base absl::strings absl::str_format_internal absl::time absl::random_random
    glog::glog gflags::gflags leveldb
)

ADD_EXECUTABLE(amdb main.h main.cc)
TARGET_LINK_LIBRARIES(amdb libamdb_sql)

IF (AMDB_BUILD_TEST)
  FOREACH (test_item ${amdb_sql_test_src})
    GET_FILENAME_COMPONENT(test_target_name ${test_item} NAME_WE)
    ADD_EXECUTABLE(${test_target_name} ${test_item})
    TARGET_LINK_LIBRARIES(${test_target_name} gtest gtest_main libamdb_sql)
    ADD_TEST(NAME ${test_target_name} COMMAND ${test_target_name})
  ENDFOREACH ()
ENDIF ()