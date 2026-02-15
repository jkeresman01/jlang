#pragma once

#include <jlang/Preprocessor/ConditionalStack.h>
#include <jlang/Preprocessor/DirectiveParser.h>
#include <jlang/Preprocessor/IncludeResolver.h>
#include <jlang/Preprocessor/MacroTable.h>

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
