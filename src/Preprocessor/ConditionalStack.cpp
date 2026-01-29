#include "Preprocessor/ConditionalStack.h"

#include <iostream>

namespace jlang
{

void ConditionalStack::HandleIfdef(bool isDefined)
{
    bool parentActive = IsActive();
    bool active = parentActive && isDefined;
    m_Stack.push({parentActive, active, active});
}

void ConditionalStack::HandleIfndef(bool isDefined)
{
    bool parentActive = IsActive();
    bool active = parentActive && !isDefined;
    m_Stack.push({parentActive, active, active});
}

void ConditionalStack::HandleElse()
{
    if (m_Stack.empty())
    {
        std::cerr << "Preprocessor error: #else without matching #ifdef/#ifndef\n";
        return;
    }

    auto &state = m_Stack.top();
    if (state.parentActive && !state.branchTaken)
    {
        state.currentlyActive = true;
        state.branchTaken = true;
    }
    else
    {
        state.currentlyActive = false;
    }
}

void ConditionalStack::HandleEndif()
{
    if (m_Stack.empty())
    {
        std::cerr << "Preprocessor error: #endif without matching #ifdef/#ifndef\n";
        return;
    }

    m_Stack.pop();
}

bool ConditionalStack::IsActive() const
{
    if (m_Stack.empty())
    {
        return true;
    }
    return m_Stack.top().currentlyActive;
}

} // namespace jlang
