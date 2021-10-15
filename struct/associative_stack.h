//
// Created by ramizouari on 15/10/2021.
//

#ifndef ALGORITHMICLIBRARY_ASSOCIATIVE_STACK_H
#define ALGORITHMICLIBRARY_ASSOCIATIVE_STACK_H
#include "binary_operator.h"
#include <stack>

template<typename T,typename O=plus_t<T>>
class associative_stack {
    using couple = std::pair<T,T>;
    std::stack<couple> S;
    inline static constexpr O F=O();
public:
    void push(const T& a)
    {
        if(S.empty())
            S.push({a,a});
        else
            S.push({a,F(S.top().second,a)});
    }
    T top() const
    {
        return S.top().first;
    }

    T query() const
    {
        return S.empty()?O::neutral:S.top().second;
    }

    void pop()
    {
        S.pop();
    }

    bool empty() const
    {
        return S.empty();
    }
};


#endif //ALGORITHMICLIBRARY_ASSOCIATIVE_STACK_H
