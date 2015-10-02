#pragma once
#include <functional>

template<typename Container1, typename Container2>
void AssertContainerEqual(const Container1 & tested, const Container2 & expected)
{
    IS_TRUE(tested.size() == expected.size());
    auto iter1 = tested.begin();
    auto iter2 = expected.begin();
    while (iter1 != tested.end())
    {
        IS_TRUE(*iter1++ == *iter2++);
    }
}


template<typename Container, typename ValueType>
void AssertContainerFillWith(const Container & c, const ValueType & val)
{
    auto iter = c.begin();
    while (iter != c.end())
    {
        IS_TRUE(*iter++ == val);
    }
}