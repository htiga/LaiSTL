#include "stdafx.h"
#include "../LaiSTL/Set.h"
#include <set>
#include <functional>

#define LAI_IMSET lai::multiset<int>
#define LAI_SMSET lai::multiset<std::string>
#define LAI_UMSET lai::multiset<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestMultiSet)
    {
        TEST_METHOD(TestDefaultContructor)
        {
        }
    };
}