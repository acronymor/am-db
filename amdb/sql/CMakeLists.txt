CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

ADD_SUBDIRECTORY(parser)

LIST(APPEND amdb_sql_folder common codec expression chunk schema context resolver analyzer plan statistics
        optimizer optimizer/costs optimizer/rules optimizer/heuristics optimizer/cascades
        scheduler executor storage
        testsuite)

FOREACH (folder ${amdb_sql_folder})
    FILE(GLOB srcs ${folder}/*.cc)
    LIST(FILTER srcs EXCLUDE REGEX ${folder}/.*test.*)
    LIST(APPEND amdb_sql_src ${srcs})

    FILE(GLOB test_srcs ${folder}/*test.cc)
    LIST(APPEND amdb_sql_test_src ${test_srcs})

    FILE(GLOB testutil_srcs ${folder}/*testutil.cc)
    LIST(APPEND amdb_sql_testutil_src ${testutil_srcs})
ENDFOREACH ()

list(APPEND amdb_sql_test_src
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser_test.cc
)

LIST(APPEND libraries absl::base absl::strings absl::str_format_internal absl::time glog::glog gflags::gflags leveldb)

ADD_LIBRARY(libamdb_sql OBJECT ${amdb_sql_src})
TARGET_LINK_LIBRARIES(libamdb_sql libamdb_proto libamdb_common libamdb_parser ${libraries})

ADD_EXECUTABLE(amdb main.cc)
TARGET_LINK_LIBRARIES(amdb libamdb_sql)

IF (AMDB_BUILD_TEST)
    ADD_DEFINITIONS(-DAMDB_BUILD_TEST)

    LIST(APPEND test_libraries gtest gtest_main absl::random_random)

    FOREACH (test_item ${amdb_sql_test_src})
        GET_FILENAME_COMPONENT(test_target_name ${test_item} NAME_WE)
        ADD_EXECUTABLE(${test_target_name} ${test_item} ${amdb_sql_testutil_src})
        TARGET_LINK_LIBRARIES(${test_target_name} libamdb_sql ${test_libraries})
        ADD_TEST(NAME ${test_target_name} COMMAND ${test_target_name})
    ENDFOREACH ()
ENDIF ()