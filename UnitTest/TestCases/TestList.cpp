#include "stdafx.h"
#include "../LaiSTL/List.h"

namespace UnitTest
{
    TEST_CLASS(TestList)
    {
        TEST_METHOD(TestInsertLvalue)
        {
            TSC_InsertLvalue(lai::list);
        }

        TEST_METHOD(TestInsertRvalue)
        {
            TSC_InsertRvalue(lai::list);
        }

        TEST_METHOD(TestEmplace)
        {
            TSC_Emplace(lai::list);
        }
    };
}