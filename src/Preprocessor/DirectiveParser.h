#pragma once

#include "Trie.h"

#include <optional>
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

struct ParsedDirective
{
    DirectiveType type;
    std::string arguments;
};

class DirectiveParser
{
  public:
    DirectiveParser();

    std::optional<ParsedDirective> Parse(const std::string &line) const;

  private:
    Trie<DirectiveType> m_Keywords;
};

} // namespace jlang
