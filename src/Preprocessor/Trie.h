#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>

namespace jlang
{

template <typename T> class Trie
{
  public:
    void Insert(const std::string &key, const T &value)
    {
        TrieNode *current = &m_Root;
        for (char c : key)
        {
            auto index = static_cast<unsigned char>(c);
            if (!current->children[index])
            {
                current->children[index] = std::make_unique<TrieNode>();
            }
            current = current->children[index].get();
        }
        current->isEndOfWord = true;
        current->value = value;
    }

    bool Remove(const std::string &key)
    {
        TrieNode *current = &m_Root;
        for (char c : key)
        {
            auto index = static_cast<unsigned char>(c);
            if (!current->children[index])
            {
                return false;
            }
            current = current->children[index].get();
        }
        if (!current->isEndOfWord)
        {
            return false;
        }
        current->isEndOfWord = false;
        current->value.reset();
        return true;
    }

    std::optional<T> Search(const std::string &key) const
    {
        const TrieNode *current = &m_Root;
        for (char c : key)
        {
            auto index = static_cast<unsigned char>(c);
            if (!current->children[index])
            {
                return std::nullopt;
            }
            current = current->children[index].get();
        }
        if (current->isEndOfWord)
        {
            return current->value;
        }
        return std::nullopt;
    }

    bool StartsWith(const std::string &prefix) const
    {
        const TrieNode *current = &m_Root;
        for (char c : prefix)
        {
            auto index = static_cast<unsigned char>(c);
            if (!current->children[index])
            {
                return false;
            }
            current = current->children[index].get();
        }
        return true;
    }

    Trie Clone() const
    {
        Trie copy;
        CloneNode(m_Root, copy.m_Root);
        return copy;
    }

  private:
    struct TrieNode
    {
        std::array<std::unique_ptr<TrieNode>, 128> children{};
        bool isEndOfWord = false;
        std::optional<T> value;
    };

    static void CloneNode(const TrieNode &src, TrieNode &dst)
    {
        dst.isEndOfWord = src.isEndOfWord;
        dst.value = src.value;
        for (size_t i = 0; i < 128; ++i)
        {
            if (src.children[i])
            {
                dst.children[i] = std::make_unique<TrieNode>();
                CloneNode(*src.children[i], *dst.children[i]);
            }
        }
    }

    TrieNode m_Root;
};

} // namespace jlang
