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
            TSC_Merge(lai::list);
        }

        TEST_METHOD(TestMergeBy)
        {
            TSC_MergeBy(lai::list);
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
            TSC_Remove(lai::list);
        }

        TEST_METHOD(TestRemoveIf)
        {
            TSC_RemoveIf(lai::list);
        }

        TEST_METHOD(TestReverse)
        {
            TSC_Reverse(lai::list);
        }

        TEST_METHOD(TestUnique)
        {
            TSC_Unique(lai::list);
        }

        TEST_METHOD(TestUniqueIf)
        {
            TSC_UniqueIf(lai::list);
        }

        TEST_METHOD(TestSort)
        {
            TSC_MemberSort(lai::list);
        }
        
        TEST_METHOD(TestSortBy)
        {
            TSC_MemberSortBy(lai::list);
        }

    };
}