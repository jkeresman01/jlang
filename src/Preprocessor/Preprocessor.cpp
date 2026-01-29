#include "Preprocessor/Preprocessor.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace jlang
{

Preprocessor::Preprocessor(const std::string &source, const std::string &filePath)
    : m_Source(source), m_FilePath(filePath)
{
    m_DirectiveKeywords.Insert("define", DirectiveType::Define);
    m_DirectiveKeywords.Insert("undef", DirectiveType::Undef);
    m_DirectiveKeywords.Insert("ifdef", DirectiveType::Ifdef);
    m_DirectiveKeywords.Insert("ifndef", DirectiveType::Ifndef);
    m_DirectiveKeywords.Insert("else", DirectiveType::Else);
    m_DirectiveKeywords.Insert("endif", DirectiveType::Endif);
    m_DirectiveKeywords.Insert("include", DirectiveType::Include);
}

std::string Preprocessor::Process()
{
    std::string output;
    std::istringstream stream(m_Source);
    std::string line;

    while (std::getline(stream, line))
    {
        ProcessLine(line, output);
    }

    return output;
}

void Preprocessor::ProcessLine(const std::string &line, std::string &output)
{
    std::string trimmed = Trim(line);

    if (!trimmed.empty() && trimmed[0] == '#')
    {
        std::string afterHash = Trim(trimmed.substr(1));

        // Extract the directive name (first word after #)
        size_t nameEnd = 0;
        while (nameEnd < afterHash.size() && !std::isspace(static_cast<unsigned char>(afterHash[nameEnd])))
        {
            ++nameEnd;
        }
        std::string directiveName = afterHash.substr(0, nameEnd);
        std::string rest = (nameEnd < afterHash.size()) ? Trim(afterHash.substr(nameEnd)) : "";

        auto directive = m_DirectiveKeywords.Search(directiveName);
        if (!directive.has_value())
        {
            std::cerr << "Preprocessor warning: unknown directive '#" << directiveName << "'\n";
            return;
        }

        // Conditional directives are always processed to maintain the stack
        switch (directive.value())
        {
        case DirectiveType::Ifdef:
            HandleIfdef(rest);
            return;
        case DirectiveType::Ifndef:
            HandleIfndef(rest);
            return;
        case DirectiveType::Else:
            HandleElse();
            return;
        case DirectiveType::Endif:
            HandleEndif();
            return;
        default:
            break;
        }

        // Other directives only processed when output is active
        if (!IsOutputActive())
        {
            return;
        }

        switch (directive.value())
        {
        case DirectiveType::Define:
            HandleDefine(rest);
            break;
        case DirectiveType::Undef:
            HandleUndef(rest);
            break;
        case DirectiveType::Include:
            HandleInclude(rest, output);
            break;
        default:
            break;
        }
        return;
    }

    // Regular line: only output if active
    if (IsOutputActive())
    {
        output += SubstituteMacros(line);
        output += '\n';
    }
}

bool Preprocessor::IsOutputActive() const
{
    if (m_ConditionalStack.empty())
    {
        return true;
    }
    return m_ConditionalStack.top().currentlyActive;
}

void Preprocessor::HandleDefine(const std::string &rest)
{
    if (rest.empty())
    {
        std::cerr << "Preprocessor error: #define requires a name\n";
        return;
    }

    // Extract macro name
    size_t nameEnd = 0;
    while (nameEnd < rest.size() && !std::isspace(static_cast<unsigned char>(rest[nameEnd])))
    {
        ++nameEnd;
    }
    std::string name = rest.substr(0, nameEnd);
    std::string value;
    if (nameEnd < rest.size())
    {
        value = Trim(rest.substr(nameEnd));
    }

    m_Macros.Insert(name, value);
}

void Preprocessor::HandleUndef(const std::string &rest)
{
    if (rest.empty())
    {
        std::cerr << "Preprocessor error: #undef requires a name\n";
        return;
    }

    std::string name = rest;
    // Take only the first token
    size_t nameEnd = 0;
    while (nameEnd < name.size() && !std::isspace(static_cast<unsigned char>(name[nameEnd])))
    {
        ++nameEnd;
    }
    name = name.substr(0, nameEnd);

    m_Macros.Remove(name);
}

void Preprocessor::HandleIfdef(const std::string &rest)
{
    bool parentActive = IsOutputActive();

    std::string name = rest;
    size_t nameEnd = 0;
    while (nameEnd < name.size() && !std::isspace(static_cast<unsigned char>(name[nameEnd])))
    {
        ++nameEnd;
    }
    name = name.substr(0, nameEnd);

    bool defined = m_Macros.Search(name).has_value();
    bool active = parentActive && defined;

    m_ConditionalStack.push({parentActive, active, active});
}

void Preprocessor::HandleIfndef(const std::string &rest)
{
    bool parentActive = IsOutputActive();

    std::string name = rest;
    size_t nameEnd = 0;
    while (nameEnd < name.size() && !std::isspace(static_cast<unsigned char>(name[nameEnd])))
    {
        ++nameEnd;
    }
    name = name.substr(0, nameEnd);

    bool defined = m_Macros.Search(name).has_value();
    bool active = parentActive && !defined;

    m_ConditionalStack.push({parentActive, active, active});
}

void Preprocessor::HandleElse()
{
    if (m_ConditionalStack.empty())
    {
        std::cerr << "Preprocessor error: #else without matching #ifdef/#ifndef\n";
        return;
    }

    auto &state = m_ConditionalStack.top();
    if (state.parentActive && !state.branchTaken)
    {
        state.currentlyActive = true;
        state.branchTaken = true;
    }
    else
    {
        state.currentlyActive = false;
    }
}

void Preprocessor::HandleEndif()
{
    if (m_ConditionalStack.empty())
    {
        std::cerr << "Preprocessor error: #endif without matching #ifdef/#ifndef\n";
        return;
    }

    m_ConditionalStack.pop();
}

void Preprocessor::HandleInclude(const std::string &rest, std::string &output)
{
    if (rest.size() < 3 || rest.front() != '"' || rest.back() != '"')
    {
        std::cerr << "Preprocessor error: #include expects \"filename\"\n";
        return;
    }

    std::string includePath = rest.substr(1, rest.size() - 2);

    // Resolve relative to the current file's directory
    std::filesystem::path currentDir = std::filesystem::path(m_FilePath).parent_path();
    std::filesystem::path resolved = currentDir / includePath;

    std::string canonical;
    try
    {
        canonical = std::filesystem::canonical(resolved).string();
    }
    catch (const std::filesystem::filesystem_error &)
    {
        std::cerr << "Preprocessor error: cannot resolve include path '" << resolved.string() << "'\n";
        return;
    }

    // Circular include guard
    if (m_IncludedFiles.count(canonical))
    {
        return;
    }
    m_IncludedFiles.insert(canonical);

    // Read the included file
    std::ifstream in(canonical);
    if (!in.is_open())
    {
        std::cerr << "Preprocessor error: cannot open include file '" << canonical << "'\n";
        return;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    // Create child preprocessor with copied state
    Preprocessor child(buffer.str(), canonical);
    child.m_Macros = m_Macros.Clone();
    child.m_IncludedFiles = m_IncludedFiles;

    std::string included = child.Process();

    // Merge state back
    m_Macros = child.m_Macros.Clone();
    m_IncludedFiles = child.m_IncludedFiles;

    output += included;
}

std::string Preprocessor::SubstituteMacros(const std::string &line) const
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

std::string Preprocessor::Trim(const std::string &str)
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
    {
        ++start;
    }
    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
    {
        --end;
    }
    return str.substr(start, end - start);
}

} // namespace jlang
