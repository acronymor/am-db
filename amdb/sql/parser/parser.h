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

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "dml.h"
#include "ddl.h"
#include "misc.h"
#include "sql/common/statuscode.h"
#include "sql/context/statement_context.h"

namespace amdb {
namespace parser {
Status GenAst(StatementContext* stmt_ctx);

struct SqlParser {
    explicit SqlParser(Arena* arena) : arena(arena) {}

    std::string charset;
    std::string collation;
    std::vector<StmtNode*> result;
    Status error = Status::C_OK;
    std::string syntax_err_str;
    Arena* arena;
    bool is_gbk = false;
    bool has_5c = false;
    int place_holder_id = 0;
    void parse(const std::string& sql);
    void change_5c_to_7f(std::string& sql);
};

inline void print_stmt(Node* node, int ii = 0) {
    std::cout << "i:" << ii << " type:" << node->node_type << " ";
    node->print();
    if (node->node_type == NT_EXPR) {
        std::cout << "to_string:" << node << std::endl;
    }
    for (int i = 0; i < node->children.size(); i++) {
        print_stmt(node->children[i], ii + 1);
    }
}
}  // namespace parser
}  // namespace amdb

/* vim: set ts=4 sw=4 sts=4 tw=100 */
