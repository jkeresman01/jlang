#include <jlang/Preprocessor/MacroTable.h>
#include <jlang/Utils/StringUtils.h>

#include <iostream>

namespace jlang
{

void MacroTable::Define(const std::string &rest)
{
    if (rest.empty())
    {
        std::cerr << "Preprocessor error: #define requires a name\n";
        return;
    }

    std::string name = StringUtils::ExtractFirstToken(rest);
    std::string value;
    if (name.size() < rest.size())
    {
        value = StringUtils::Trim(rest.substr(name.size()));
    }

    m_Macros.Insert(name, value);
}

void MacroTable::Undef(const std::string &rest)
{
    if (rest.empty())
    {
        std::cerr << "Preprocessor error: #undef requires a name\n";
        return;
    }

    m_Macros.Remove(StringUtils::ExtractFirstToken(rest));
}

bool MacroTable::IsDefined(const std::string &name) const
{
    return m_Macros.Search(name).has_value();
}

std::string MacroTable::Substitute(const std::string &line) const
{
    std::string result;
    size_t i = 0;
    size_t len = line.size();

    while (i < len)
    {
        // Skip // line comments
        if (i + 1 < len && line[i] == '/' && line[i + 1] == '/')
        {
            result += line.substr(i);
            break;
        }

        // Skip string literals
        if (line[i] == '"')
        {
            result += line[i++];
            while (i < len && line[i] != '"')
            {
                if (line[i] == '\\' && i + 1 < len)
                {
                    result += line[i++];
                }
                result += line[i++];
            }
            if (i < len)
            {
                result += line[i++]; // closing "
            }
            continue;
        }

        // Skip char literals
        if (line[i] == '\'')
        {
            result += line[i++];
            while (i < len && line[i] != '\'')
            {
                if (line[i] == '\\' && i + 1 < len)
                {
                    result += line[i++];
                }
                result += line[i++];
            }
            if (i < len)
            {
                result += line[i++]; // closing '
            }
            continue;
        }

        // Identifier: [a-zA-Z_][a-zA-Z0-9_]*
        if (std::isalpha(static_cast<unsigned char>(line[i])) || line[i] == '_')
        {
            size_t start = i;
            while (i < len && (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_'))
            {
                ++i;
            }
            std::string identifier = line.substr(start, i - start);

            auto macro = m_Macros.Search(identifier);
            if (macro.has_value())
            {
                result += macro.value();
            }
            else
            {
                result += identifier;
            }
            continue;
        }

        result += line[i++];
    }

    return result;
}

MacroTable MacroTable::Clone() const
{
    MacroTable copy;
    copy.m_Macros = m_Macros.Clone();
    return copy;
}

} // namespace jlang
