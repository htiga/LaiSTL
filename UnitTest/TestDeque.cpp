#include "stdafx.h"
#include "../LaiSTL/Deque.h"
#include <deque>

#define LAI_IDEQ lai::deque<int>
#define LAI_SDEQ lai::deque<std::string>
#define LAI_UDEQ lai::deque<Uncopyable>
#define STD_IDEQ std::deque<int>
#define STD_SDEQ std::deque<std::string>
#define STD_UDEQ std::deque<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestDequeIterator)
    {

    };

    TEST_CLASS(TestDeque)
    {
        TEST_METHOD(TestDefaultConstructor)
        {
            TC_DefaultConstructor<LAI_IDEQ>();
            TC_DefaultConstructor<LAI_SDEQ>();
            TC_DefaultConstructor<LAI_UDEQ>();
        }

        TEST_METHOD(TestCountConstructor)
        {
            TC_CountConstructor<LAI_IDEQ>();
            TC_CountConstructor<LAI_UDEQ>();
            TC_CountConstructor<LAI_SDEQ>(std::string("lai"));
        }

        TEST_METHOD(TestRangeConstructor)
        {
            TC_RangeConstructor<LAI_IDEQ>();
            TC_RangeConstructor<LAI_SDEQ>();
        }

        TEST_METHOD(TestBracedListConstructor)
        {
            TC_BracedListConstructor(lai::deque);
        }

        TEST_METHOD(TestReize)
        {
            TC_Resize(lai::deque);
        }

        TEST_METHOD(TestResizeVal)
        {
            TC_ResizeVal(lai::deque);
        }
    };
}