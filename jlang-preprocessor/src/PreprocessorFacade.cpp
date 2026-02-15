#include <jlang/Preprocessor/PreprocessorFacade.h>
#include <jlang/Utils/StringUtils.h>

#include <sstream>

namespace jlang
{

PreprocessorFacade::PreprocessorFacade(const std::string &source, const std::string &filePath)
    : m_Source(source), m_FilePath(filePath)
{
}

std::string PreprocessorFacade::Process()
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

void PreprocessorFacade::ProcessLine(const std::string &line, std::string &output)
{
    auto directive = m_DirectiveParser.Parse(line);
    if (directive.has_value())
    {
        auto &[type, rest] = directive.value();

        switch (type)
        {
        case DirectiveType::Ifdef:
            m_Conditionals.HandleIfdef(m_Macros.IsDefined(StringUtils::ExtractFirstToken(rest)));
            return;
        case DirectiveType::Ifndef:
            m_Conditionals.HandleIfndef(m_Macros.IsDefined(StringUtils::ExtractFirstToken(rest)));
            return;
        case DirectiveType::Else:
            m_Conditionals.HandleElse();
            return;
        case DirectiveType::Endif:
            m_Conditionals.HandleEndif();
            return;
        default:
            break;
        }

        if (!m_Conditionals.IsActive())
        {
            return;
        }

        switch (type)
        {
        case DirectiveType::Define:
            m_Macros.Define(rest);
            break;
        case DirectiveType::Undef:
            m_Macros.Undef(rest);
            break;
        case DirectiveType::Include:
            HandleInclude(rest, output);
            break;
        default:
            break;
        }
        return;
    }

    if (m_Conditionals.IsActive())
    {
        output += m_Macros.Substitute(line);
        output += '\n';
    }
}

void PreprocessorFacade::HandleInclude(const std::string &rest, std::string &output)
{
    auto resolved = m_IncludeResolver.Resolve(rest, m_FilePath);
    if (!resolved)
    {
        return;
    }

    PreprocessorFacade child(resolved->content, resolved->canonicalPath);
    child.m_Macros = m_Macros.Clone();
    child.m_IncludeResolver.SetIncludedFiles(m_IncludeResolver.IncludedFiles());

    std::string included = child.Process();

    m_Macros = child.m_Macros.Clone();
    m_IncludeResolver.SetIncludedFiles(child.m_IncludeResolver.IncludedFiles());

    output += included;
}

} // namespace jlang
