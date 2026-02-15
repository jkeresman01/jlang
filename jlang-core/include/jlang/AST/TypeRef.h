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
    bool isArray = false;
    int arraySize = 0;
    std::vector<TypeRef> typeParameters;

    bool isGeneric() const { return !typeParameters.empty(); }

    bool isResult() const { return name == "Result" && typeParameters.size() == 2; }

    bool isArrayType() const { return isArray && arraySize > 0; }

    std::string getMangledName() const
    {
        std::string mangled = name;

        if (isArrayType())
        {
            mangled += "_arr" + std::to_string(arraySize);
        }

        if (isGeneric())
        {
            for (const auto &param : typeParameters)
            {
                mangled += "_" + param.getMangledName();
            }
        }

        if (isPointer)
        {
            mangled += "_ptr";
        }
        return mangled;
    }
};

} // namespace jlang
