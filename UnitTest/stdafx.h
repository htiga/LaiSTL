// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


// Headers for CppUnitTest
#include "CppUnitTest.h"


// TODO: reference additional headers your program requires here
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#define IS_TRUE Assert::IsTrue
#define IS_FALSE Assert::IsFalse

template<typename Container1, typename Container2>
void AssertContainerEqual(const Container1 & tested, const Container2 & expected)
{
    IS_TRUE(tested.size() == expected.size());
    for (int i = 0; i != tested.size(); ++i)
    {
        IS_TRUE(tested[i] == expected[i]);
    }
}


template<typename Container, typename Object>
void AssertContainerFilled(const Container & c, const Object & filledVal)
{
    for (int i = 0; i != c.size(); ++i)
    {
        IS_TRUE(c[i] == filledVal);
    }
}
