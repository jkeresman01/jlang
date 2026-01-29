#pragma once

#include "../AST/Ast.h"
#include "../AST/Expressions/Expressions.h"
#include "../AST/Statements/Statements.h"
#include "../AST/TopLevelDecl/TopLevelDecl.h"

#include <memory>
#include <optional>
#include <string>

namespace jlang
{

struct LoopUnrollInfo
{
    std::string varName;
    int64_t start;
    int64_t end;
    int64_t step;          // +1 or -1
    std::string compareOp; // "<", "<=", ">", ">="
};

class LoopUnroller
{
  public:
    static const int MAX_UNROLL_COUNT = 8;

    static std::optional<LoopUnrollInfo> AnalyzeForUnroll(ForStatement &node);
    static bool BodyModifiesVar(const std::shared_ptr<AstNode> &node, const std::string &varName);
};

} // namespace jlang
