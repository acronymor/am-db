#include "column_description.h"

namespace amdb {
namespace chunk {
ColumnDescriptor::ColumnDescriptor(expr::ExprValueType type, uint32_t col_id)
    : type_(type), col_id_(col_id) {}

uint32_t ColumnDescriptor::ID() { return col_id_; }

expr::ExprValueType ColumnDescriptor::Type() { return type_; }

uint8_t ColumnDescriptor::TypeSize() const { return expr::TypeSize(type_); }
}  // namespace chunk
}  // namespace amdb