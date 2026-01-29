#pragma once

#include "Trie.h"

#include <set>
#include <stack>
#include <string>

namespace jlang
{

enum class DirectiveType
{
    Define,
    Undef,
    Ifdef,
    Ifndef,
    Else,
    Endif,
    Include
};

class Preprocessor
{
  public:
    Preprocessor(const std::string &source, const std::string &filePath);

    std::string Process();

  private:
    struct ConditionalState
    {
        bool parentActive;
        bool branchTaken;
        bool currentlyActive;
    };

    void ProcessLine(const std::string &line, std::string &output);
    bool IsOutputActive() const;

    void HandleDefine(const std::string &rest);
    void HandleUndef(const std::string &rest);
    void HandleIfdef(const std::string &rest);
    void HandleIfndef(const std::string &rest);
    void HandleElse();
    void HandleEndif();
    void HandleInclude(const std::string &rest, std::string &output);

    std::string SubstituteMacros(const std::string &line) const;

    static std::string Trim(const std::string &str);

  private:
    std::string m_Source;
    std::string m_FilePath;

    Trie<DirectiveType> m_DirectiveKeywords;
    Trie<std::string> m_Macros;

    std::stack<ConditionalState> m_ConditionalStack;
    std::set<std::string> m_IncludedFiles;
};

} // namespace jlang
