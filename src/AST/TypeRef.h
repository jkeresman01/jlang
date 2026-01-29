#pragma once

#include <string>

namespace jlang
{

struct TypeRef
{
    std::string name;
    bool isPointer = false;
    bool isNullable = false;
};

} // namespace jlang
