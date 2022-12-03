CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

BISON_TARGET(MyParser sql_parse.y ${CMAKE_CURRENT_BINARY_DIR}/sql_parse.yacc.cc)
FLEX_TARGET(MyScanner sql_lex.l ${CMAKE_CURRENT_BINARY_DIR}/sql_lex.flex.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

FILE(GLOB_RECURSE libamdb_parser_SRC "*.cc")

LIST(FILTER libamdb_parser_SRC EXCLUDE REGEX ${folder}/.*test.*)

ADD_LIBRARY(libamdb_parser ${libamdb_parser_SRC} ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})
TARGET_COMPILE_OPTIONS(libamdb_parser BEFORE PRIVATE "-Wno-sign-compare")
TARGET_LINK_LIBRARIES(libamdb_parser libamdb_common protobuf::libprotobuf)

IF (NOT DEFINED APPLE)
  TARGET_LINK_LIBRARIES(libamdb_parser ${FLEX_LIBRARIES} ${BISON_LIBRARIES})
ELSE ()
  MESSAGE(WARNING "Ignore link libs in MACOS to continue cmake, which means parser not runnable.")
endif ()

MESSAGE("scanner outputs ${FLEX_MyScanner_OUTPUTS}")
MESSAGE("parser outputs ${BISON_MyParser_OUTPUTS}")
MESSAGE("flex_bison libraries ${FLEX_LIBRARIES} ${BISON_LIBRARIES}")