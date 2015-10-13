#include "stdafx.h"
#include "../LaiSTL/List.h"

#define LAI_ILST lai::list<int>
#define LAI_SLST lai::list<std::string>

namespace UnitTest
{
    TEST_CLASS(TestList)
    {
        TEST_METHOD(TestDefaultConstructor)
        {
            TSC_DefaultConstructor(lai::list);
        }

        TEST_METHOD(TestCountConstructor)
        {
            TSC_CountConstructor(lai::list);
        }

        TEST_METHOD(TestRangeConstructor)
        {
            TSC_RangeConstructor(lai::list);
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            TSC_BraceListConstructor(lai::list);
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::list);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::list);
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::list);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::list);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::list);
        }

        TEST_METHOD(TestAssignCount)
        {
            TSC_AssignCount(lai::list);
        }

        TEST_METHOD(TestAssignRange)
        {
            TSC_AssignRange(lai::list);
        }

        TEST_METHOD(TestAssignInitList)
        {
            TSC_AssignInitList(lai::list);
        }

        TEST_METHOD(TestFront)
        {
            TSC_Front(lai::list);
        }

        TEST_METHOD(TestBack)
        {
            TSC_Back(lai::list);
        }

        TEST_METHOD(TestInsertLvalue)
        {
            TSC_InsertLvalue(lai::list);
        }

        TEST_METHOD(TestInsertRvalue)
        {
            TSC_InsertRvalue(lai::list);
        }

        TEST_METHOD(TestInsertCount)
        {
            TSC_InsertCount(lai::list);
        }

        TEST_METHOD(TestInsertRange)
        {
            TSC_InsertRange(lai::list);
        }

        TEST_METHOD(TestInsertInitList)
        {
            TSC_InsertInitList(lai::list);
        }

        TEST_METHOD(TestEmplace)
        {
            TSC_Emplace(lai::list);
        }

        TEST_METHOD(TestErase)
        {
            TSC_Erase(lai::list);
        }

        TEST_METHOD(TestEraseRange)
        {
            TSC_EraseRange(lai::list);
        }

        TEST_METHOD(TestPushBackLvalue)
        {
            TSC_PushBackLvalue(lai::list);
        }

        TEST_METHOD(TestPushBackRvalue)
        {
            TSC_PushBackRvalue(lai::list);
        }

        TEST_METHOD(TestEmplaceBack)
        {
            TSC_EmplaceBack(lai::list);
        }

        TEST_METHOD(TestPopBack)
        {
            TSC_PopBack(lai::list);
        }

        TEST_METHOD(TestPushFrontLvalue)
        {
            TSC_PushFrontLvalue(lai::list);
        }

        TEST_METHOD(TestPushFrontRvalue)
        {
            TSC_PushFrontRvalue(lai::list);
        }

        TEST_METHOD(TestEmplaceFront)
        {
            TSC_EmplaceFront(lai::list);
        }

        TEST_METHOD(TestPopFront)
        {
            TSC_PopFront(lai::list);
        }

        TEST_METHOD(TestResize)
        {
            TSC_Resize(lai::list);
        }

        TEST_METHOD(TestResizeVal)
        {
            TSC_ResizeVal(lai::list);
        }

        TEST_METHOD(TestMemberSwap)
        {
            TSC_MemberSwap(lai::list);
        }

        TEST_METHOD(TestNonMemberSwap)
        {
            TSC_NonMemberSwap(lai::list, lai::swap);
        }

        TEST_METHOD(TestRelationalOperator)
        {
            TSC_RelationalOperator(lai::list);
        }

        TEST_METHOD(TestMerge)
        {
            LAI_ILST a, b;

            a.merge(b);
            AssertContainerEqual(a, I_IL{});
            AssertContainerEqual(b, I_IL{});

            a = { 1,2,3,7,8,9 };
            a.merge(b);
            AssertContainerEqual(a, I_IL{ 1,2,3,7,8,9 });
            AssertContainerEqual(b, I_IL{});

            a = {};
            b = { 1,2,3,7,8,9 };
            a.merge(b);
            AssertContainerEqual(a, I_IL{ 1,2,3,7,8,9 });
            AssertContainerEqual(b, I_IL{});

            b = { 4,5 };
            a.merge(b);
            AssertContainerEqual(a, I_IL{ 1,2,3,4,5,7,8,9 });
            AssertContainerEqual(b, I_IL{});

            b = { 0, 6, 10 };
            a.merge(b);
            AssertContainerEqual(a, I_IL{ 0,1,2,3,4,5,6,7,8,9,10 });
            AssertContainerEqual(b, I_IL{});
        }

        TEST_METHOD(TestMergeComp)
        {
            LAI_ILST a, b;

            auto comp = std::greater<int>();

            a.merge(b, comp);
            AssertContainerEqual(a, I_IL{});
            AssertContainerEqual(b, I_IL{});

            a = { 9,8,7,3,2,1 };
            a.merge(b, comp);
            AssertContainerEqual(a, I_IL{ 9,8,7,3,2,1 });
            AssertContainerEqual(b, I_IL{});

            a = {};
            b = { 9,8,7,3,2,1 };
            a.merge(b, comp);
            AssertContainerEqual(a, I_IL{ 9,8,7,3,2,1 });
            AssertContainerEqual(b, I_IL{});

            b = { 5,4 };
            a.merge(b, comp);
            AssertContainerEqual(a, I_IL{ 9,8,7,5,4,3,2,1 });
            AssertContainerEqual(b, I_IL{});

            b = { 10, 6, 0 };
            a.merge(b, comp);
            AssertContainerEqual(a, I_IL{ 10,9,8,7,6,5,4,3,2,1,0 });
            AssertContainerEqual(b, I_IL{});
        }

        TEST_METHOD(TestSplice1)
        {
            LAI_ILST a, b;

            a.splice(a.begin(), b);
            AssertContainerEqual(a, I_IL{});
            AssertContainerEqual(b, I_IL{});

            b = { 1,2,3 };
            a.splice(a.begin(), b);
            AssertContainerEqual(a, I_IL{ 1,2,3 });
            AssertContainerEqual(b, I_IL{});

            b = { 4,5,6 };
            a.splice(++a.begin(), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,3 });
            AssertContainerEqual(b, I_IL{});

            b = { 7,8 };
            a.splice(a.end(), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,3,7,8 });
            AssertContainerEqual(b, I_IL{});

            b = { 9 };
            a.splice(--a.end(), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,3,7,9,8 });
            AssertContainerEqual(b, I_IL{});
        }

        TEST_METHOD(TestSplice2)
        {
            LAI_ILST a, b;

            b = { 1,2,3,4,5 };
            a.splice(a.begin(), b, b.begin());
            AssertContainerEqual(a, I_IL{ 1 });
            AssertContainerEqual(b, I_IL{ 2,3,4,5 });

            a.splice(++a.begin(), b, b.begin());
            AssertContainerEqual(a, I_IL{ 1,2 });
            AssertContainerEqual(b, I_IL{ 3,4,5 });

            a.splice(--a.end(), b, b.begin());
            AssertContainerEqual(a, I_IL{ 1,3,2 });
            AssertContainerEqual(b, I_IL{ 4,5 });

            a.splice(a.end(), b, b.begin());
            AssertContainerEqual(a, I_IL{ 1,3,2,4 });
            AssertContainerEqual(b, I_IL{ 5 });

            // test splice on self

            a.splice(a.begin(), a, ++a.begin());
            AssertContainerEqual(a, I_IL{ 3,1,2,4 });

            a.splice(a.end(), a, a.begin());
            AssertContainerEqual(a, I_IL{ 1,2,4,3 });

            a.splice(++a.begin(), a, a.begin());
            AssertContainerEqual(a, I_IL{ 1,2,4,3 });

            a.splice(--a.end(), a, ++a.begin());
            AssertContainerEqual(a, I_IL{ 1,4,2,3 });
        }

        TEST_METHOD(TestSplice3)
        {
            LAI_ILST a, b;

            a.splice(a.begin(), b, b.begin(), b.end());
            AssertContainerEqual(a, I_IL{ });
            AssertContainerEqual(b, I_IL{ });

            b = { 1,2,3,4,5,6,7,8 };
            a.splice(a.begin(), b, b.begin(), std::next(b.begin(), 2));
            AssertContainerEqual(a, I_IL{1,2});
            AssertContainerEqual(b, I_IL{ 3,4,5,6,7,8 });

            a.splice(++a.begin(), b, b.begin(), std::next(b.begin(), 2));
            AssertContainerEqual(a, I_IL{ 1,3,4,2 });
            AssertContainerEqual(b, I_IL{ 5,6,7,8 });

            a.splice(--a.end(), b, b.begin(), std::next(b.begin(), 1));
            AssertContainerEqual(a, I_IL{ 1,3,4,5,2 });
            AssertContainerEqual(b, I_IL{ 6,7,8 });

            a.splice(a.end(), b, b.begin(), b.end());
            AssertContainerEqual(a, I_IL{ 1,3,4,5,2,6,7,8 });
            AssertContainerEqual(b, I_IL{});

            // test splice on self

            a.splice(a.begin(), a, ++a.begin(), a.end());
            AssertContainerEqual(a, I_IL{ 3,4,5,2,6,7,8,1 });
            
            a.splice(std::next(a.end(), -3), a, std::next(a.end(), -2), a.end());
            AssertContainerEqual(a, I_IL{ 3,4,5,2,6,8,1,7 });
        }

        TEST_METHOD(TestRemove)
        {
            LAI_ILST a;

            a.remove(0);
            AssertContainerEqual(a, I_IL{});

            a = { 0,0,0,0 };
            a.remove(1);
            AssertContainerEqual(a, I_IL{ 0,0,0,0 });

            a.remove(0);
            AssertContainerEqual(a, I_IL{});

            a = { 0, 1, 1, 2, 3, 0, 4, 5, 0, 5, 5, 5};
            a.remove(0);
            AssertContainerEqual(a, I_IL{ 1, 1, 2, 3, 4, 5, 5, 5, 5 });

            a.remove(1);
            AssertContainerEqual(a, I_IL{ 2, 3, 4, 5, 5, 5, 5 });
            
            a.remove(5);
            AssertContainerEqual(a, I_IL{ 2, 3, 4 });
        }

        TEST_METHOD(TestRemoveIf)
        {
            LAI_ILST a;

            a.remove_if([](int i) {return true; });
            AssertContainerEqual(a, I_IL{});

            a = { 0,1,2,3,4,5,6,7,8,9 };

            a.remove_if([](int i) {return i > 7; });
            AssertContainerEqual(a, I_IL{ 0,1,2,3,4,5,6,7 });

            a.remove_if([](int i) { return i < 2; });
            AssertContainerEqual(a, I_IL{ 2,3,4,5,6,7 });

            a.remove_if([](int i) { return false; });
            AssertContainerEqual(a, I_IL{ 2,3,4,5,6,7 });

            a.remove_if([](int i) { return i % 2 == 0; });
            AssertContainerEqual(a, I_IL{ 3,5,7 });

            a.remove_if([](int i) { return true; });
            AssertContainerEqual(a, I_IL{ });
        }

        TEST_METHOD(TestReverse)
        {
            LAI_ILST il = {};

            il.reverse();
            AssertContainerEqual(il, I_IL{});

            il = { 0 };
            il.reverse();
            AssertContainerEqual(il, I_IL{ 0 });

            il = { 0, 0 };
            il.reverse();
            AssertContainerEqual(il, I_IL{ 0, 0 });

            il = { 1, 2, 3, 0, 3, 2, 1 };
            il.reverse();
            AssertContainerEqual(il, I_IL{ 1, 2, 3, 0, 3, 2, 1 });
            il.reverse();
            AssertContainerEqual(il, I_IL{ 1, 2, 3, 0, 3, 2, 1 });

            il = { 1,2,3,4,5,6,7,8,9 };
            il.reverse();
            AssertContainerEqual(il, I_IL{ 9,8,7,6,5,4,3,2,1 });
            il.reverse();
            AssertContainerEqual(il, I_IL{ 1,2,3,4,5,6,7,8,9 });
        }

        TEST_METHOD(TestUnique)
        {
            LAI_ILST a;

            a.unique();
            AssertContainerEqual(a, I_IL{});

            a = { 1 };
            a.unique();
            AssertContainerEqual(a, I_IL{ 1 });

            a = { 1, 1 };
            a.unique();
            AssertContainerEqual(a, I_IL{ 1 });

            a = { 1,1,1,1,1,1 };
            a.unique();
            AssertContainerEqual(a, I_IL{ 1 });

            a = { 1, 1, 0, 1 };
            a.unique();
            AssertContainerEqual(a, I_IL{ 1,0,1 });

            a = { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 0, 0, 1, 1, 2, 2, 3, 3 };
            a.unique();
            AssertContainerEqual(a, I_IL{ 0,1,2,3,0,1,2,3 });
        }

        TEST_METHOD(TestUniqueIf)
        {
            LAI_ILST a;

            a.unique([](int i, int j) {return true; });
            AssertContainerEqual(a, I_IL{});

            a = { 0, 1,2, 4,3, 5,6, 8,7, 9 };
            a.unique([](int i, int j) { return i > j; });
            AssertContainerEqual(a, I_IL{ 0, 1,2, 4, 5,6, 8, 9 });

            a.unique([](int i, int j) { return i == j - 1; });
            AssertContainerEqual(a, I_IL{ 0, 2, 4, 6, 8 });

            a.unique([](int i, int j) { return false; });
            AssertContainerEqual(a, I_IL{ 0, 2, 4, 6, 8 });

            a.unique([](int i, int j) { return true; });
            AssertContainerEqual(a, I_IL{ 0 });
        }

        TEST_METHOD(TestSort1)
        {
            std::initializer_list<I_IL> testData =
            {
                {},
                {0},
                {0, 0},
                {0, 0, 0},
                {0, 1},
                {1, 0},
                {0, 1, 2},
                {0, 2, 1},
                {1, 0, 2},
                {1, 2, 0},
                {2, 0, 1},
                {2, 1, 0},
                {0, 1, 1},
                {1, 0, 1},
                {1, 1, 0},
                {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
                {42, 9, 17, 54, 602, -3, 54, 999, -11},
                {-11, -3, 9, 17, 42, 54, 54, 602, 999},
            };

            std::initializer_list<I_IL> sortedData =
            {
                {},
                { 0 },
                { 0, 0 },
                { 0, 0, 0 },
                { 0, 1 },
                { 0, 1 },
                { 0, 1, 2 },
                { 0, 1, 2 },
                { 0, 1, 2 },
                { 0, 1, 2 },
                { 0, 1, 2 },
                { 0, 1, 2 },
                { 0, 1, 1 },
                { 0, 1, 1 },
                { 0, 1, 1 },
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
                { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
            };

            auto testIter = testData.begin();
            auto stdIter = sortedData.begin();
            while (testIter != testData.end())
            {
                LAI_ILST il = *testIter++;
                il.sort();
                AssertContainerEqual(il, *stdIter++);
            }
        }
        
        TEST_METHOD(TestSort2)
        {
            std::initializer_list<I_IL> testData =
            {
                {},
                { 0 },
                { 0, 0 },
                { 0, 0, 0 },
                { 0, 1 },
                { 1, 0 },
                { 0, 1, 2 },
                { 0, 2, 1 },
                { 1, 0, 2 },
                { 1, 2, 0 },
                { 2, 0, 1 },
                { 2, 1, 0 },
                { 0, 1, 1 },
                { 1, 0, 1 },
                { 1, 1, 0 },
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
                { 42, 9, 17, 54, 602, -3, 54, 999, -11 },
                { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
            };

            std::initializer_list<I_IL> sortedData =
            {
                {},
                { 0 },
                { 0, 0 },
                { 0, 0, 0 },
                { 1, 0 },
                { 1, 0 },
                { 2, 1, 0 },
                { 2, 1, 0 },
                { 2, 1, 0 },
                { 2, 1, 0 },
                { 2, 1, 0 },
                { 2, 1, 0 },
                { 1, 1, 0 },
                { 1, 1, 0 },
                { 1, 1, 0 },
                { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
                { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
                { 999, 602, 54, 54, 42, 17, 9, -3, -11 },
                { 999, 602, 54, 54, 42, 17, 9, -3, -11 },
            };

            auto testIter = testData.begin();
            auto stdIter = sortedData.begin();
            while (testIter != testData.end())
            {
                LAI_ILST il = *testIter++;
                il.sort([](int i, int j) { return i >= j; });
                AssertContainerEqual(il, *stdIter++);
            }
        }

    };
}