#pragma once

#include <string>

namespace jlang
{

class StringUtils
{
  public:
    StringUtils() = delete;

    static inline std::string Trim(const std::string &str)
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

    static inline std::string ExtractFirstToken(const std::string &str)
    {
        size_t end = 0;
        while (end < str.size() && !std::isspace(static_cast<unsigned char>(str[end])))
        {
            ++end;
        }
        return str.substr(0, end);
    }
};

} // namespace jlang
