#pragma once

#include <string>
#include <vector>

namespace jlang
{

struct TypeRef
{
    std::string name;
    bool isPointer = false;
    bool isNullable = false;
    std::vector<TypeRef> typeParameters;

    bool isGeneric() const { return !typeParameters.empty(); }

    bool isResult() const { return name == "Result" && typeParameters.size() == 2; }

    std::string getMangledName() const
    {
        if (!isGeneric())
        {
            return name + (isPointer ? "_ptr" : "");
        }

        std::string mangled = name;
        for (const auto &param : typeParameters)
        {
            mangled += "_" + param.getMangledName();
        }
        if (isPointer)
        {
            mangled += "_ptr";
        }
        return mangled;
    }
};

} // namespace jlang
