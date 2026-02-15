#pragma once

#include <jlang/Preprocessor/Trie.h>

#include <string>

namespace jlang
{

class MacroTable
{
  public:
    void Define(const std::string &rest);
    void Undef(const std::string &rest);
    bool IsDefined(const std::string &name) const;
    std::string Substitute(const std::string &line) const;

    MacroTable Clone() const;

  private:
    Trie<std::string> m_Macros;
};

} // namespace jlang
