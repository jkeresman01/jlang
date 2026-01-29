#pragma once

#include "ConditionalStack.h"
#include "DirectiveParser.h"
#include "IncludeResolver.h"
#include "MacroTable.h"

#include <string>

namespace jlang
{

class PreprocessorFacade
{
  public:
    PreprocessorFacade(const std::string &source, const std::string &filePath);

    std::string Process();

  private:
    void ProcessLine(const std::string &line, std::string &output);
    void HandleInclude(const std::string &rest, std::string &output);

    std::string m_Source;
    std::string m_FilePath;

    DirectiveParser m_DirectiveParser;
    ConditionalStack m_Conditionals;
    MacroTable m_Macros;
    IncludeResolver m_IncludeResolver;
};

} // namespace jlang
