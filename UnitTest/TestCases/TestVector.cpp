#include "stdafx.h"
#include "../../LaiSTL/Vector.h"


#define LAI_IVEC lai::vector<int>
#define LAI_SVEC lai::vector<std::string>
#define LAI_UVEC lai::vector<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestVectorIterator)
    {
        TEST_METHOD(TestCategory)
        {
            IS_TRUE(IteratorCategoryTraits<LAI_IVEC::iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IVEC::const_iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IVEC::reverse_iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IVEC::const_reverse_iterator>::isRandomAccess);
        }

        // todo
    };

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

        TEST_METHOD(TestFront)
        {
            TSC_Front(lai::vector);
        }

        TEST_METHOD(TestBack)
        {
            TSC_Back(lai::vector);
        }

        TEST_METHOD(TestCapacityOperations)
        {
            int size = 3;
            LAI_UVEC vec(size);

            IS_FALSE(vec.empty());
            IS_TRUE(vec.size() == size);

            vec.reserve(20);
            IS_FALSE(vec.empty());
            IS_TRUE(vec.size() == size);
            IS_TRUE(vec.capacity() == 20);

            vec.clear();
            IS_TRUE(vec.empty());
            IS_TRUE(vec.size() == 0);
            IS_TRUE(vec.capacity() == 20);

            vec.shrink_to_fit();
            IS_TRUE(vec.empty());
            IS_TRUE(vec.size() == 0);
            IS_TRUE(vec.capacity() == 0);
        }

        TEST_METHOD(TestInsertLvalue)
        {
            TSC_InsertLvalue(lai::vector);
            
            // test insert an exited element

            LAI_SVEC svec{ "a" };
            for (int i = 0; i != 20; ++i)
            {
                svec.insert(svec.begin(), svec[0]);
            }
            AssertContainerEqual(svec, STD_SVEC{ 21, "a" });
        }

        TEST_METHOD(TestInsertRvalue)
        {
            TSC_InsertRvalue(lai::vector);
        }

        TEST_METHOD(TestInsertCount)
        {
            TSC_InsertCount(lai::vector);

            // test insert existed elements.

            std::size_t count = 15;
            LAI_SVEC ss{ "a" };
            ss.insert(ss.begin(), count, ss[0]);
            AssertContainerEqual(ss, STD_SVEC{ count + 1, "a" });
        }

        TEST_METHOD(TestInsertRange)
        {
            TSC_InsertRange(lai::vector);
        }

        TEST_METHOD(TestInsertInitList)
        {
            TSC_InsertInitList(lai::vector);
        }

        TEST_METHOD(TestEmplace)
        {
            TSC_Emplace(lai::vector);
        }

        TEST_METHOD(TestErase)
        {
            TSC_Erase(lai::vector);
        }

        TEST_METHOD(TestEraseRange)
        {
            TSC_EraseRange(lai::vector);
        }

        TEST_METHOD(TestPushBackLvalue)
        {
            TSC_PushBackLvalue(lai::vector);

            // test if push_back an element existed in the vector is valid.

            int size = 30;
            std::string data[] = { "lai", "stl" };
            LAI_SVEC s{ "lai", "stl" };

            for (int i = 0; i != size; ++i)
            {
                s.push_back(s[i]);
            }

            for (int i = 0; i != size + 2; ++i)
            {
                IS_TRUE(s[i] == data[i % 2]);
            }
        }

        TEST_METHOD(TestPushBackRvalue)
        {
            TSC_PushBackRvalue(lai::vector);

            // test if push_back an element existed in the vector is valid.

            int size = 30;
            LAI_UVEC uVec;
            STD_UVEC stdV;
            for (int i = 0; i != size; ++i)
            {
                uVec.push_back(Uncopyable(i));
                stdV.push_back(Uncopyable(i));
            }

            for (int i = 0; i != size; ++i)
            {
                uVec.push_back(std::move(uVec[i]));
            }

            for (int i = size; i != size * 2; ++i)
            {
                IS_TRUE(uVec[i] == stdV[i - size]);
            }
        }

        TEST_METHOD(TestEmplaceBack)
        {
            TSC_EmplaceBack(lai::vector);
        }


        TEST_METHOD(TestPopBack)
        {
            TSC_PopBack(lai::vector);
        }

        //TEST_METHOD(TestResize)
        //{
        //    TSC_Resize(lai::vector);
        //}

        //TEST_METHOD(TestResizeVal)
        //{
        //    TSC_ResizeVal(lai::vector);
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
    };
}