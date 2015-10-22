#include "stdafx.h"
#include "../LaiSTL/Forward_List.h"
#include <forward_list>

#define LAI_IFLST lai::forward_list<int>
#define LAI_SFLST lai::forward_list<std::string>
#define LAI_UFLST lai::forward_list<Uncopyable>

#define STD_IFLST std::forward_list<int>
#define STD_SFLST std::forward_list<std::string>
#define STD_UFLST std::forward_list<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestForwardList)
    {
        TEST_METHOD(TestDefaultConstructor)
        {
            TSC_DefaultConstructor(lai::forward_list);
        }

        TEST_METHOD(TestCountConstructor)
        {
            TSC_CountConstructor(lai::forward_list);
        }

        TEST_METHOD(TestRangeConstructor)
        {
            TSC_RangeConstructor(lai::forward_list);
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            TSC_BraceListConstructor(lai::forward_list);
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::forward_list);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::forward_list);
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::forward_list);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::forward_list);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::forward_list);
        }

        TEST_METHOD(TestAssignCount)
        {
            TSC_AssignCount(lai::forward_list);
        }

        TEST_METHOD(TestAssignRange)
        {
            TSC_AssignRange(lai::forward_list);
        }

        TEST_METHOD(TestAssignInitList)
        {
            TSC_AssignInitList(lai::forward_list);
        }

        TEST_METHOD(TestFront)
        {
            TSC_Front(lai::forward_list);
        }

        TEST_METHOD(TestInsertAfterLvalue)
        {
            I_IL offsets = { 0,1,5,10,14 };

            I_IL intData = { 0,1,2,3,4,5,6,7,8,9 };
            int insertedInt = 42;

            LAI_IFLST il = intData;
            STD_IFLST stdIl = intData;

            for (auto & off : offsets)
            {
                auto iter = il.insert_after(std::next(il.before_begin(), off), insertedInt);
                stdIl.insert_after(std::next(stdIl.before_begin(), off), insertedInt);

                IS_TRUE(iter == std::next(il.before_begin(), off + 1));
                AssertContainerEqual(il, stdIl);
            }

            S_IL strData = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
            std::string insertedStr = "laistl";

            LAI_SFLST sl = strData;
            STD_SFLST stdSl = strData;

            for (auto & off : offsets)
            {
                auto iter = sl.insert_after(std::next(sl.before_begin(), off), insertedStr);
                stdSl.insert_after(std::next(stdSl.before_begin(), off), insertedStr);

                IS_TRUE(iter == std::next(sl.before_begin(), off + 1));
                AssertContainerEqual(sl, stdSl);
            }
        }

        TEST_METHOD(TestInsertAfterRvalue)
        {
            I_IL offsets = { 0,1,5,10,14 };

            LAI_UFLST ul(10);
            STD_UFLST stdUl(10);

            for (auto & off : offsets)
            {
                auto iter = ul.insert_after(std::next(ul.before_begin(), off), Uncopyable(42));
                stdUl.insert_after(std::next(stdUl.before_begin(), off), Uncopyable(42));

                IS_TRUE(iter == std::next(ul.before_begin(), off + 1));
                AssertContainerEqual(ul, stdUl);
            }
        }

        TEST_METHOD(TestInsertAfterCount)
        {

            I_IL intData = { 0,1,2,3,4,5,6,7,8,9 };
            I_IL offsets = { 0,1,5,9 };
            std::initializer_list<std::size_t> counts = { 0, 1, 3, 5 };
            int insertedInt = 42;

            LAI_IFLST il = intData;
            STD_IFLST stdIl = intData;

            for (auto & off : offsets)
            {
                for (auto & count : counts)
                {
                    auto iter = il.insert_after(std::next(il.before_begin(), off), count, insertedInt);
                    stdIl.insert_after(std::next(stdIl.before_begin(), off), count, insertedInt);

                    IS_TRUE(iter == std::next(il.before_begin(), off + count));
                    AssertContainerEqual(il, stdIl);
                }
            }
        }

        TEST_METHOD(TestInsertAfterRange)
        {
            S_IL strData = { "a", "b", "c", "d", "e", "f", "g", "h"};
            LAI_SFLST sl;

            auto iter = sl.insert_after(sl.before_begin(), strData.begin(), strData.begin());
            IS_TRUE(iter == sl.before_begin());
            IS_TRUE(sl.empty());

            iter = sl.insert_after(sl.before_begin(), strData.begin(), strData.begin() + 1);
            IS_TRUE(iter == sl.begin());
            AssertContainerEqual(sl, S_IL{ "a" });

            iter = sl.insert_after(sl.begin(), strData.begin(), strData.begin());
            IS_TRUE(iter == sl.begin());
            AssertContainerEqual(sl, S_IL{ "a" });

            iter = sl.insert_after(sl.begin(), strData.begin() + 1, strData.begin() + 3);
            IS_TRUE(*iter == "c");
            AssertContainerEqual(sl, S_IL{ "a", "b", "c" });

            iter = sl.insert_after(std::next(sl.begin(), 1), strData.begin() + 3, strData.begin() + 5);
            IS_TRUE(*iter == "e");
            AssertContainerEqual(sl, S_IL{ "a", "b", "d", "e", "c" });

            iter = sl.insert_after(std::next(sl.begin(), 4), strData.begin() + 5, strData.begin() + 8);
            IS_TRUE(*iter == "h");
            AssertContainerEqual(sl, S_IL{ "a", "b", "d", "e", "c", "f", "g", "h" });
        }

        TEST_METHOD(TestInsertAfterInitList)
        {
            STD_SFLST sl;

            auto iter = sl.insert_after(sl.before_begin(), {});
            IS_TRUE(iter == sl.before_begin());
            IS_TRUE(sl.empty());

            iter = sl.insert_after(sl.before_begin(), { "a" });
            IS_TRUE(*iter == "a");
            AssertContainerEqual(sl, S_IL{ "a" });

            iter = sl.insert_after(sl.begin(), {});
            IS_TRUE(iter == sl.begin());
            AssertContainerEqual(sl, S_IL{ "a" });

            iter = sl.insert_after(sl.begin(), { "b", "c" });
            IS_TRUE(*iter == "c");
            AssertContainerEqual(sl, S_IL{ "a", "b", "c" });

            iter = sl.insert_after(std::next(sl.begin(), 1), { "d", "e" });
            IS_TRUE(*iter == "e");
            AssertContainerEqual(sl, S_IL{ "a", "b", "d", "e", "c" });

            iter = sl.insert_after(std::next(sl.begin(), 4), { "f", "g", "h" });
            IS_TRUE(*iter == "h");
            AssertContainerEqual(sl, S_IL{ "a", "b", "d", "e", "c", "f", "g", "h" });
        }

        TEST_METHOD(TestEmplaceAfter)
        {
            LAI_UFLST ul;

            auto iter = ul.emplace_after(ul.before_begin());
            IS_TRUE(iter == ul.begin());
            AssertContainerEqual(ul, U_IL{ Uncopyable() });

            iter = ul.emplace_after(ul.begin(), Uncopyable(1));
            IS_TRUE(*iter == Uncopyable(1));
            AssertContainerEqual(ul, U_IL{ Uncopyable(), Uncopyable(1) });


            LAI_SFLST sl;

            auto iter1 = sl.emplace_after(sl.before_begin());
            IS_TRUE(iter1 == sl.begin());
            AssertContainerEqual(sl, S_IL{ "" });

            iter1 = sl.emplace_after(sl.begin(), "a");
            IS_TRUE(*iter1 == "a");
            AssertContainerEqual(sl, S_IL{ "", "a" });

            iter1 = sl.emplace_after(std::next(sl.begin(), 1), 2, 'b');
            IS_TRUE(*iter1 == "bb");
            AssertContainerEqual(sl, S_IL{ "", "a", "bb" });

            std::string str = "c";
            iter1 = sl.emplace_after(std::next(sl.begin(), 2), str);
            IS_TRUE(*iter1 == "c");
            AssertContainerEqual(sl, S_IL{ "", "a", "bb", "c" });

            iter1 = sl.emplace_after(std::next(sl.begin(), 2), std::move(str));
            IS_TRUE(iter1 == std::next(sl.begin(), 3));
            AssertContainerEqual(sl, S_IL{ "", "a", "bb", "c", "c" });

            std::initializer_list<char> il = { 'l', 'a', 'i' };
            iter1 = sl.emplace_after(std::next(sl.begin(), 1), il);
            IS_TRUE(iter1 == std::next(sl.begin(), 2));
            AssertContainerEqual(sl, S_IL{ "", "a", "lai", "bb", "c", "c" });
        }

        TEST_METHOD(TestEraseAfter)
        {
            STD_IFLST c = { 0,1,2,3,4,5,6,7,8,9 };

            auto iter = c.erase_after(c.before_begin());
            IS_TRUE(iter == c.begin());
            AssertContainerEqual(c, I_IL{ 1,2,3,4,5,6,7,8,9 });
               
            iter = c.erase_after(c.begin());
            IS_TRUE(iter == std::next(c.begin()));
            AssertContainerEqual(c, I_IL{ 1,3,4,5,6,7,8,9 });

            iter = c.erase_after(std::next(c.begin(), 1));
            IS_TRUE(iter == std::next(c.begin(), 2));
            AssertContainerEqual(c, I_IL{ 1,3,5,6,7,8,9 });

            iter = c.erase_after(std::next(c.begin(), 4));
            IS_TRUE(iter == std::next(c.begin(), 5));
            AssertContainerEqual(c, I_IL{ 1,3,5,6,7,9 });

            iter = c.erase_after(std::next(c.begin(), 4));
            IS_TRUE(iter == c.end());
            AssertContainerEqual(c, I_IL{ 1,3,5,6,7 });
        }

        TEST_METHOD(TestPushFrontLvalue)
        {
            TSC_PushFrontLvalue(lai::forward_list);
        }

        TEST_METHOD(TestPushFrontRvalue)
        {
            TSC_PushFrontRvalue(lai::forward_list);
        }

        TEST_METHOD(TestEmplaceFront)
        {
            TSC_EmplaceFront(lai::forward_list);
        }

        TEST_METHOD(TestPopFront)
        {
            TSC_PopFront(lai::forward_list);
        }

        TEST_METHOD(TestResize)
        {
            TSC_Resize(lai::forward_list);
        }

        TEST_METHOD(TestResizeVal)
        {
            TSC_ResizeVal(lai::forward_list);
        }

        TEST_METHOD(TestMemberSwap)
        {
            TSC_MemberSwap(lai::forward_list);
        }

        TEST_METHOD(TestNonMemberSwap)
        {
            TSC_NonMemberSwap(lai::forward_list, lai::swap);
        }

        TEST_METHOD(TestRelationalOperator)
        {
            TSC_RelationalOperator(lai::forward_list);
        }

        TEST_METHOD(TestMerge)
        {
            TSC_Merge(lai::forward_list);
        }

        TEST_METHOD(TestMergeBy)
        {
            TSC_MergeBy(lai::forward_list);
        }

        TEST_METHOD(TestSpliceAfterAll)
        {
            LAI_IFLST a, b;

            a.splice_after(a.before_begin(), b);
            AssertContainerEqual(a, I_IL{});
            AssertContainerEqual(b, I_IL{});

            b = { 1,2,3 };
            a.splice_after(a.before_begin(), b);
            AssertContainerEqual(a, I_IL{ 1,2,3 });
            AssertContainerEqual(b, I_IL{});

            b = { 4,5,6 };
            a.splice_after(a.begin(), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,3 });
            AssertContainerEqual(b, I_IL{});

            b = { 7,8 };
            a.splice_after(std::next(a.begin(), 4), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,7,8,3 });
            AssertContainerEqual(b, I_IL{});

            b = { 9 };
            a.splice_after(std::next(a.begin(), 7), b);
            AssertContainerEqual(a, I_IL{ 1,4,5,6,2,7,8,3,9 });
            AssertContainerEqual(b, I_IL{});
        }

        TEST_METHOD(TestSpliceAfterOne)
        {
            LAI_IFLST a, b;

            b = { 1,2,3,4,5 };
            a.splice_after(a.before_begin(), b, b.before_begin());
            AssertContainerEqual(a, I_IL{ 1 });
            AssertContainerEqual(b, I_IL{ 2,3,4,5 });

            a.splice_after(a.begin(), b, b.before_begin());
            AssertContainerEqual(a, I_IL{ 1,2 });
            AssertContainerEqual(b, I_IL{ 3,4,5 });

            a.splice_after(a.begin(), b, b.before_begin());
            AssertContainerEqual(a, I_IL{ 1,3,2 });
            AssertContainerEqual(b, I_IL{ 4,5 });

            a.splice_after(std::next(a.begin()), b, b.before_begin());
            AssertContainerEqual(a, I_IL{ 1,3,4,2 });
            AssertContainerEqual(b, I_IL{ 5 });

            a.splice_after(std::next(a.begin(),3), b, b.before_begin());
            AssertContainerEqual(a, I_IL{ 1,3,4,2,5 });
            AssertContainerEqual(b, I_IL{ });

            // test splice_after on self

            a.splice_after(a.begin(), a, a.before_begin());
            AssertContainerEqual(a, I_IL{ 1,3,4,2,5 });

            a.splice_after(a.begin(), a, a.begin());
            AssertContainerEqual(a, I_IL{ 1,3,4,2,5 });

            a.splice_after(a.before_begin(), a, a.begin());
            AssertContainerEqual(a, I_IL{ 3,1,4,2,5 });

            a.splice_after(std::next(a.begin(), 4), a, a.begin());
            AssertContainerEqual(a, I_IL{ 3,4,2,5,1 });

            a.splice_after(a.begin(), a, std::next(a.begin(), 3));
            AssertContainerEqual(a, I_IL{ 3,1,4,2,5 });
        }

        TEST_METHOD(TestSpliceAfterRange)
        {
            LAI_IFLST a, b;

            a.splice_after(a.before_begin(), b, b.before_begin(), b.end());
            AssertContainerEqual(a, I_IL{});
            AssertContainerEqual(b, I_IL{});

            b = { 1,2,3,4,5,6,7,8 };
            a.splice_after(a.before_begin(), b, b.before_begin(), std::next(b.begin(), 2));
            AssertContainerEqual(a, I_IL{ 1,2 });
            AssertContainerEqual(b, I_IL{ 3,4,5,6,7,8 });

            a.splice_after(a.begin(), b, b.before_begin(), std::next(b.begin(), 2));
            AssertContainerEqual(a, I_IL{ 1,3,4,2 });
            AssertContainerEqual(b, I_IL{ 5,6,7,8 });

            a.splice_after(std::next(a.begin(), 2), b, b.before_begin(), std::next(b.begin(), 1));
            AssertContainerEqual(a, I_IL{ 1,3,4,5,2 });
            AssertContainerEqual(b, I_IL{ 6,7,8 });

            a.splice_after(std::next(a.begin(), 4), b, b.before_begin(), b.end());
            AssertContainerEqual(a, I_IL{ 1,3,4,5,2,6,7,8 });
            AssertContainerEqual(b, I_IL{});

            // test splice_after on self

            a.splice_after(a.before_begin(), a, a.begin(), a.end());
            AssertContainerEqual(a, I_IL{ 3,4,5,2,6,7,8,1 });

            a.splice_after(std::next(a.begin(), 7), a, a.before_begin(), std::next(a.begin(), 7));
            AssertContainerEqual(a, I_IL{ 1,3,4,5,2,6,7,8 });
        }

        TEST_METHOD(TestRemove)
        {
            TSC_Remove(lai::forward_list);
        }

        TEST_METHOD(TestRemoveIf)
        {
            TSC_RemoveIf(lai::forward_list);
        }

        TEST_METHOD(TestReverse)
        {
            TSC_Reverse(lai::forward_list);
        }

        TEST_METHOD(TestUnique)
        {
            TSC_Unique(lai::forward_list);
        }

        TEST_METHOD(TestUniqueIf)
        {
            TSC_UniqueIf(lai::forward_list);
        }

        TEST_METHOD(TestSort)
        {
            TSC_MemberSort(lai::forward_list);
        }

        TEST_METHOD(TestSortBy)
        {
            TSC_MemberSortBy(lai::forward_list);
        }
    };
}