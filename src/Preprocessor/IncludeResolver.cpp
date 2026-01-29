#include "Preprocessor/IncludeResolver.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace jlang
{

std::optional<ResolvedInclude> IncludeResolver::Resolve(const std::string &rest,
                                                        const std::string &currentFilePath)
{
    if (rest.size() < 3 || rest.front() != '"' || rest.back() != '"')
    {
        std::cerr << "Preprocessor error: #include expects \"filename\"\n";
        return std::nullopt;
    }

    std::string includePath = rest.substr(1, rest.size() - 2);

    std::filesystem::path currentDir = std::filesystem::path(currentFilePath).parent_path();
    std::filesystem::path resolved = currentDir / includePath;

    std::string canonical;
    try
    {
        canonical = std::filesystem::canonical(resolved).string();
    }
    catch (const std::filesystem::filesystem_error &)
    {
        std::cerr << "Preprocessor error: cannot resolve include path '" << resolved.string() << "'\n";
        return std::nullopt;
    }

    if (m_IncludedFiles.count(canonical))
    {
        return std::nullopt;
    }
    m_IncludedFiles.insert(canonical);

    std::ifstream in(canonical);
    if (!in.is_open())
    {
        std::cerr << "Preprocessor error: cannot open include file '" << canonical << "'\n";
        return std::nullopt;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    return ResolvedInclude{buffer.str(), canonical};
}

const std::set<std::string> &IncludeResolver::IncludedFiles() const
{
    return m_IncludedFiles;
}

void IncludeResolver::SetIncludedFiles(std::set<std::string> files)
{
    m_IncludedFiles = std::move(files);
}

} // namespace jlang
