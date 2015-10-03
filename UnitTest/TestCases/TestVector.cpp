#include "stdafx.h"
#include "../../LaiSTL/Vector.h"


#define LAI_IVEC lai::vector<int>
#define LAI_SVEC lai::vector<std::string>
#define LAI_UVEC lai::vector<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestVector)
    {
    public:
        TEST_METHOD(TestDefaultConstructor)
        {
            TSC_DefaultConstructor(lai::vector);
        }

        TEST_METHOD(TestCountConstructor)
        {
            TSC_CountConstructor(lai::vector);
        }

        TEST_METHOD(TestRangeContructor)
        {
            TSC_RangeConstructor(lai::vector);
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            TSC_BraceListConstructor(lai::vector);
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::vector);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::vector);
        }

        TEST_METHOD(CopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::vector);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::vector);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::vector);
        }

        TEST_METHOD(TestAssignCount)
        {
            TSC_AssignCount(lai::vector);
        }

        TEST_METHOD(TestAssignRange)
        {
            TSC_AssignRange(lai::vector);
        }

        TEST_METHOD(TestAssignInitList)
        {
            TSC_AssignInitList(lai::vector);
        }

        TEST_METHOD(TestAt)
        {
            TSC_At(lai::vector);
        }

        TEST_METHOD(TestSubscript)
        {
            TSC_Subscript(lai::vector);
        }

        //TEST_METHOD(TestPushBack)
        //{
        //    TSC_PushBack(lai::vector);

        //    // test : push_back an element existed in the vector is valid.
        //    //        Reallocation may invalidate this operation.

        //    int size = 100;
        //    std::string data[] = { "lai", "stl" };
        //    LAI_SVEC s{ "lai", "stl" };

        //    for (int i = 0; i != size; ++i)
        //    {
        //        s.push_back(s[i]);
        //    }

        //    for (int i = 0; i != size + 2; ++i)
        //    {
        //        IS_TRUE(s[i] == data[i % 2]);
        //    }
        //}

        //TEST_METHOD(TestMovePushBack)
        //{
        //    TSC_MovePushBack(lai::vector);

        //    // test : push_back an element existed in the vector is valid.
        //    //        Reallocation may invalidate this operation.

        //    int size = 50;
        //    LAI_UVEC uVec;
        //    STD_UVEC stdV;
        //    for (int i = 0; i != size; ++i)
        //    {
        //        uVec.push_back(Uncopyable(i));
        //        stdV.push_back(Uncopyable(i));
        //    }

        //    for (int i = 0; i != size; ++i)
        //    {
        //        uVec.push_back(std::move(uVec[i]));
        //    }

        //    for (int i = size; i != size * 2; ++i)
        //    {
        //        IS_TRUE(uVec[i] == stdV[i - size]);
        //    }
        //}

        //TEST_METHOD(TestEmplaceBack)
        //{
        //    std::string s("a");
        //    LAI_SVEC v;
        //    v.emplace_back(1, 'a');
        //    v.emplace_back("a");
        //    v.emplace_back(s);
        //    v.emplace_back(std::move(s));
        //    AssertContainerEqual(v, STD_SVEC{4, "a"});

        //    LAI_UVEC v1;
        //    v1.emplace_back(Uncopyable());
        //    IS_TRUE(v1.size() == 1);
        //}


        //TEST_METHOD(TestPopBack)
        //{
        //    TSC_PopBack(lai::vector, std::vector);
        //}


        //TEST_METHOD(TestErase)
        //{
        //    TSC_Erase(lai::vector);
        //}

        //TEST_METHOD(TestEraseRange)
        //{
        //    TSC_EraseRange(lai::vector);
        //}

        //TEST_METHOD(TestResize)
        //{
        //    TSC_Resize(lai::vector);
        //}

        //TEST_METHOD(TestResizeVal)
        //{
        //    TSC_ResizeVal(lai::vector);
        //}


        //TEST_METHOD(TestEmplace)
        //{
        //    TSC_Emplace(lai::vector);
        //}

        //TEST_METHOD(TestInsert)
        //{
        //    TSC_Insert(lai::vector);
        //    
        //    // test : insert an exited element
        //    LAI_SVEC ss{ "a" };
        //    for (int i = 0; i != 20; ++i)
        //        ss.insert(ss.begin(), ss[0]);
        //    AssertContainerEqual(ss, STD_SVEC{ 21, "a" });
        //}

        //TEST_METHOD(TestInsertCount)
        //{
        //    TSC_InsertCount(lai::vector, std::vector);

        //    // test : insert existed elements.
        //    // test if reallocation invalidate this operation.
        //    std::size_t count = 15;
        //    LAI_SVEC ss{ "a" };
        //    ss.insert(ss.begin(), count, ss[0]);
        //    AssertContainerEqual(ss, STD_SVEC{ count + 1, "a" });
        //}

        //TEST_METHOD(TestInsertInitList)
        //{
        //    TSC_InsertInitList(lai::vector);

        //    LAI_SVEC v;
        //    // test if reallocation invalidate this operation.
        //    for (int i = 0; i != 20; ++i)
        //    {
        //        v.insert(v.end(), { "a" });
        //    }
        //    AssertContainerEqual(v, STD_SVEC{ 20, "a" });
        //}


        //TEST_METHOD(TestInsertRange)
        //{
        //    TSC_InsertRange(lai::vector, std::vector);

        //    LAI_SVEC v;
        //    STD_SVEC stdV;
        //    std::initializer_list<std::string> il =
        //    {
        //        "lai", "stl"
        //    };
        //    for (int i = 0; i != 20; ++i)
        //    {
        //        v.insert(v.begin(), il.begin(), il.end());
        //        stdV.insert(stdV.begin(), il.begin(), il.end());
        //    }
        //    AssertContainerEqual(v, stdV);

        //}

        //TEST_METHOD(TestSwap)
        //{
        //    TSC_Swap(lai::vector);
        //    TSC_NonmemberSwap(lai::vector, lai::swap);
        //}

        //TEST_METHOD(TestRelationalOperators)
        //{
        //    TSC_RelationalOperator(lai::vector);
        //}
        

        //TEST_METHOD(TestCapacityOperations)
        //{
        //    int size = 3;
        //    LAI_UVEC vec(size);

        //    IS_FALSE(vec.empty());
        //    IS_TRUE(vec.size() == size);

        //    vec.reserve(20);
        //    IS_FALSE(vec.empty());
        //    IS_TRUE(vec.size() == size);
        //    IS_TRUE(vec.capacity() == 20);

        //    vec.clear();
        //    IS_TRUE(vec.empty());
        //    IS_TRUE(vec.size() == 0);
        //    IS_TRUE(vec.capacity() == 20);

        //    vec.shrink_to_fit();
        //    IS_TRUE(vec.empty());
        //    IS_TRUE(vec.size() == 0);
        //    IS_TRUE(vec.capacity() == 0);

        //}
    };
}