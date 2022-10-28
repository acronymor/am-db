// Copyright (c) 2018-present Baidu, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// https://github.com/baidu/BaikalDB/blob/master/include/sqlparser/ddl.h

namespace amdb {
namespace schema {

const int MYSQL_FLOAT_PRECISION = 24;

enum CharacterSet : unsigned char {
  CHARSET_ASCII = 0,
  CHARSET_UTF8,
  CHARSET_GBK,
  CHARSET_LATIN1,
  CHARSET_BINARY,
  CHARSET_NULL,
};

enum MysqlType : unsigned char {
  MYSQL_TYPE_DECIMAL = 0,
  MYSQL_TYPE_TINY,
  MYSQL_TYPE_SHORT,
  MYSQL_TYPE_LONG,
  MYSQL_TYPE_FLOAT,
  MYSQL_TYPE_DOUBLE,
  MYSQL_TYPE_NULL,
  MYSQL_TYPE_TIMESTAMP,
  MYSQL_TYPE_LONGLONG,
  MYSQL_TYPE_INT24,
  MYSQL_TYPE_DATE,
  MYSQL_TYPE_TIME,
  MYSQL_TYPE_DATETIME,
  MYSQL_TYPE_YEAR,
  MYSQL_TYPE_NEWDATE,
  MYSQL_TYPE_VARCHAR,
  MYSQL_TYPE_BIT,
  MYSQL_TYPE_TIMESTAMP2,
  MYSQL_TYPE_DATETIME2,
  MYSQL_TYPE_TIME2,
  MYSQL_TYPE_TDIGEST = 242,
  MYSQL_TYPE_BITMAP = 243,
  MYSQL_TYPE_HLL = 244,
  MYSQL_TYPE_JSON = 245,
  MYSQL_TYPE_NEWDECIMAL = 246,
  MYSQL_TYPE_ENUM = 247,
  MYSQL_TYPE_SET = 248,
  MYSQL_TYPE_TINY_BLOB = 249,
  MYSQL_TYPE_MEDIUM_BLOB = 250,
  MYSQL_TYPE_LONG_BLOB = 251,
  MYSQL_TYPE_BLOB = 252,
  MYSQL_TYPE_VAR_STRING = 253,
  MYSQL_TYPE_STRING = 254,
  MYSQL_TYPE_GEOMETRY = 255
};

// https://github.com/mysql/mysql-server/blob/8.0/include/mysql_com.h
enum MysqlFieldFlag {
  MYSQL_FIELD_FLAG_NOT_NULL = 1 << 0,
  MYSQL_FIELD_FLAG_PRI_KEY = 1 << 1,
  MYSQL_FIELD_FLAG_UNIQ_KEY = 1 << 2,
  MYSQL_FIELD_FLAG_MULTI_KEY = 1 << 3,
  MYSQL_FIELD_FLAG_BLOB = 1 << 4,
  MYSQL_FIELD_FLAG_UNSIGNED = 1 << 5,
  MYSQL_FIELD_FLAG_ZEROFILL = 1 << 6,
  MYSQL_FIELD_FLAG_BINARY = 1 << 7,
  MYSQL_FIELD_FLAG_ENUM = 1 << 8,
  MYSQL_FIELD_FLAG_AUTO_INC = 1 << 9,
  MYSQL_FIELD_FLAG_Timestamp = 1 << 10,
  MYSQL_FIELD_FLAG_SET = 1 << 11,
  MYSQL_FIELD_FLAG_NODEFAULTVALUE = 1 << 12,
  MYSQL_FIELD_FLAG_ONUPDATENOW = 1 << 13,
  MYSQL_FIELD_FLAG_PARTKEY = 1 << 14,
  MYSQL_FIELD_FLAG_NUM = 1 << 15
};

enum ColumnOptionType : unsigned char {
  COLUMN_OPT_NONE = 0,
  COLUMN_OPT_NULL,
  COLUMN_OPT_NOT_NULL,
  COLUMN_OPT_AUTO_INC,
  COLUMN_OPT_DEFAULT_VAL,
  COLUMN_OPT_PRIMARY_KEY,
  COLUMN_OPT_UNIQ_KEY,
  COLUMN_OPT_FULLTEXT,
  COLUMN_OPT_ON_UPDATE,
  COLUMN_OPT_COMMENT,
  COLUMN_OPT_GENERATED,
  COLUMN_OPT_REFERENCE
};

enum ConstraintType : unsigned char {
  CONSTRAINT_NONE = 0,
  CONSTRAINT_PRIMARY,
  CONSTRAINT_INDEX,
  CONSTRAINT_UNIQ,
  CONSTRAINT_FOREIGN_KEY,
  CONSTRAINT_FULLTEXT
};

enum IndexDistibuteType : unsigned char {
  INDEX_DIST_DEFAULT = 0,
  INDEX_DIST_LOCAL,
  INDEX_DIST_GLOBAL
};

enum TableOptionType : unsigned char {
  TABLE_OPT_NONE = 0,
  TABLE_OPT_ENGINE,
  TABLE_OPT_CHARSET,
  TABLE_OPT_COLLATE,
  TABLE_OPT_AUTO_INC,
  TABLE_OPT_COMMENT,
  TABLE_OPT_AVG_ROW_LENGTH,
  TABLE_OPT_CHECKSUM,
  TABLE_OPT_COMPRESSION,
  TABLE_OPT_CONNECTION,
  TABLE_OPT_PASSWORD,
  TABLE_OPT_KEY_BLOCK_SIZE,
  TABLE_OPT_MAX_ROWS,
  TABLE_OPT_MIN_ROWS,
  TABLE_OPT_DELAY_KEY_WRITE,
  TABLE_OPT_ROW_FORMAT,
  TABLE_OPT_STATS_PERSISTENT,
  TABLE_OPT_SHARD_ROW_ID,
  TABLE_OPT_PACK_KEYS,
  TABLE_OPT_PARTITION
};

enum DatabaseOptionType : unsigned char {
  DATABASE_OPT_NONE = 0,
  DATABASE_OPT_CHARSET,
  DATABASE_OPT_COLLATE
};

// https://dev.mysql.com/doc/refman/8.0/en/alter-table.html
enum AlterSpecType : unsigned char {
  ALTER_SPEC_ADD_COLUMN = 0,  // only support add column at the tail
  ALTER_SPEC_ADD_INDEX,
  ALTER_SPEC_ADD_CONSTRAINT,
  ALTER_SPEC_ADD_FULLTEXT,

  ALTER_SPEC_DROP_COLUMN,
  ALTER_SPEC_DROP_INDEX,

  ALTER_SPEC_RENAME_COLUMN,
  ALTER_SPEC_RENAME_TABLE,
  ALTER_SPEC_TABLE_OPTION,
  ALTER_SPEC_RESTORE_INDEX,
  ALTER_SPEC_ADD_LEARNER,
  ALTER_SPEC_DROP_LEARNER,
  ALTER_SPEC_MODIFY_COLUMN,
  ALTER_SPEC_SWAP_TABLE
};

enum PartitionType : unsigned char { PARTITION_HASH = 0, PARTITION_RANGE };

};  // namespace schema
}  // namespace amdb
   /* vim: set ts=4 sw=4 sts=4 tw=100 */