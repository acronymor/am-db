CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)

FILE(GLOB proto_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

SET(PROTO_CC_FILES)
SET(PROTO_HDR_FILES)

FOREACH(proto ${proto_files})
  STRING(REGEX REPLACE ".proto\$" ".pb.h" hdr_file "${proto}")
  STRING(REGEX REPLACE ".proto\$" ".pb.cc" cc_file "${proto}")
  MESSAGE(STATUS "add proto target: ${proto} -> ${hdr_file}, ${cc_file}")

  SET(input_proto_file ${CMAKE_CURRENT_SOURCE_DIR}/${proto})
  SET(output_hdr_file ${CMAKE_CURRENT_BINARY_DIR}/${hdr_file})
  SET(output_cc_file ${CMAKE_CURRENT_BINARY_DIR}/${cc_file})

  ADD_CUSTOM_COMMAND(OUTPUT "${output_cc_file}" "${output_hdr_file}"
      COMMAND protobuf::protoc -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ${input_proto_file}
      DEPENDS "${input_proto_file}" protobuf::protoc
      COMMENT "generate proto file ${cc_file}, ${hdr_file} at ${CMAKE_CURRENT_BINARY_DIR}"
      )

  SET(PROTO_HDR_FILES ${PROTO_HDR_FILES} "${output_hdr_file}")
  SET(PROTO_CC_FILES ${PROTO_CC_FILES} "${output_cc_file}")
ENDFOREACH()

ADD_CUSTOM_TARGET(proto_gen DEPENDS ${PROTO_HDR_FILES} ${PROTO_CC_FILES})

ADD_LIBRARY(libamdb_proto ${PROTO_CC_FILES} ${PROTO_HDR_FILES})
TARGET_LINK_LIBRARIES(libamdb_proto protobuf::libprotobuf)
# need to inlcude with "proto/xx.pb.h"