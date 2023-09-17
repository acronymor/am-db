#include <string>
#include <unordered_map>

#include "sql/schema/schema.h"
#include "sql/testsuite/kv_data_testutil.h"

namespace amdb {
namespace testsuite {

const std::string kDbName = "test_db";
const std::string kTableName = "test_table";

class SchemaGen : public KvDataTest {
 public:
  SchemaGen() : KvDataTest(){};
  ~SchemaGen() = default;

 public:
  schema::DatabaseInfo* GenDatabaseInfo(uint64_t db_id);
  schema::TableInfo* GenTableInfo(uint64_t db_id, uint64_t table_id);

  schema::DatabaseInfo* GetDatabaseInfo(uint64_t db_id);
  schema::TableInfo* GetTableInfo(uint64_t db_id, uint64_t table_id);

  Status DumpMata(uint64_t db_id, uint64_t table_id);

 protected:
  inline std::string toName(uint64_t id, const std::string& name) { return absl::StrFormat("%s_%d", name, id); }
  inline std::string toDbKey(uint64_t id) { return absl::StrFormat("%d", id); }
  inline std::string toTableKey(uint64_t db_id, uint64_t table_id) { return absl::StrFormat("%d_%d", db_id, table_id); }

 private:
  std::unordered_map<std::string, schema::DatabaseInfo*> dbs_;
  std::unordered_map<std::string, schema::TableInfo*> tables_;
};
}  // namespace testsuite
}  // namespace amdb