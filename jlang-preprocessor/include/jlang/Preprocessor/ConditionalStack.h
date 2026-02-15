#pragma once

#include <stack>

namespace jlang
{

class ConditionalStack
{
  public:
    void HandleIfdef(bool isDefined);
    void HandleIfndef(bool isDefined);
    void HandleElse();
    void HandleEndif();
    bool IsActive() const;

  private:
    struct ConditionalState
    {
        bool parentActive;
        bool branchTaken;
        bool currentlyActive;
    };

    std::stack<ConditionalState> m_Stack;
};

} // namespace jlang
