#include "Preprocessor/DirectiveParser.h"
#include "Utils/StringUtils.h"

#include <iostream>

namespace jlang
{

DirectiveParser::DirectiveParser()
{
    m_Keywords.Insert("define", DirectiveType::Define);
    m_Keywords.Insert("undef", DirectiveType::Undef);
    m_Keywords.Insert("ifdef", DirectiveType::Ifdef);
    m_Keywords.Insert("ifndef", DirectiveType::Ifndef);
    m_Keywords.Insert("else", DirectiveType::Else);
    m_Keywords.Insert("endif", DirectiveType::Endif);
    m_Keywords.Insert("include", DirectiveType::Include);
}

std::optional<ParsedDirective> DirectiveParser::Parse(const std::string &line) const
{
    std::string trimmed = StringUtils::Trim(line);
    if (trimmed.empty() || trimmed[0] != '#')
    {
        return std::nullopt;
    }

    std::string afterHash = StringUtils::Trim(trimmed.substr(1));

    size_t nameEnd = 0;
    while (nameEnd < afterHash.size() && !std::isspace(static_cast<unsigned char>(afterHash[nameEnd])))
    {
        ++nameEnd;
    }

    std::string directiveName = afterHash.substr(0, nameEnd);
    std::string rest = (nameEnd < afterHash.size()) ? StringUtils::Trim(afterHash.substr(nameEnd)) : "";

    auto directive = m_Keywords.Search(directiveName);
    if (!directive.has_value())
    {
        std::cerr << "Preprocessor warning: unknown directive '#" << directiveName << "'\n";
        return std::nullopt;
    }

    return ParsedDirective{directive.value(), rest};
}

} // namespace jlang
