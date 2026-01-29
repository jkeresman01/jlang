#pragma once

#include <optional>
#include <set>
#include <string>

namespace jlang
{

struct ResolvedInclude
{
    std::string content;
    std::string canonicalPath;
};

class IncludeResolver
{
  public:
    std::optional<ResolvedInclude> Resolve(const std::string &rest, const std::string &currentFilePath);

    const std::set<std::string> &IncludedFiles() const;
    void SetIncludedFiles(std::set<std::string> files);

  private:
    std::set<std::string> m_IncludedFiles;
};

} // namespace jlang
