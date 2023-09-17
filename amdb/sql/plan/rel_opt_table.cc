#include "sql/plan/rel_opt_table.h"

namespace amdb {
namespace plan {

double RelOptTable::GetRows() const { return rows_; }
void RelOptTable::Init(schema::DatabaseInfo *schema, schema::TableInfo *table) {
  this->schema_ = schema;
  this->table_ = table;
}
schema::TableInfo *RelOptTable::GetTable() const { return table_; };

schema::DatabaseInfo *RelOptTable::GetDB() const { return schema_; };
}  // namespace plan
}  // namespace amdb
