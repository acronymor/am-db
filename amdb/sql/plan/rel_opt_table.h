#pragma once

#include "sql/schema/schema.h"

namespace amdb {
namespace plan {

class RelOptTable {
 public:
  RelOptTable() = default;

  void Init(schema::DatabaseInfo* schema, schema::TableInfo* table);

  double GetRows() const;
  schema::TableInfo* GetTable() const;
  schema::DatabaseInfo* GetDB() const;

 private:
  schema::DatabaseInfo* schema_;
  schema::TableInfo* table_;
  double rows_;
};
}  // namespace plan
}  // namespace amdb
