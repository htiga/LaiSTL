#include "stdafx.h"
#include "../LaiSTL/Set.h"
#include <set>
#include <functional>

#define LAI_IMSET lai::multiset<int>
#define LAI_SMSET lai::multiset<std::string>
#define LAI_UMSET lai::multiset<Uncopyable>

namespace UnitTest
{
    namespace
    {
        std::initializer_list<I_IL> G_UnsortedData =
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

        std::initializer_list<I_IL> G_SortedData =
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

        std::initializer_list<I_IL> G_ReverseSortedData =
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
    }

    TEST_CLASS(TestMultiSet)
    {
        TEST_METHOD(TestDefaultContructor)
        {
            TSC_DefaultConstructor(lai::multiset);

            auto greater = std::greater<>();
            TSC_DefaultConstructorAux<lai::multiset<int, std::greater<>>>();
            TSC_DefaultConstructorAux<lai::multiset<std::string, std::greater<>>>();
            TSC_DefaultConstructorAux<lai::multiset<Uncopyable, std::greater<>>>();
        }

        TEST_METHOD(TestRangeConstructor)
        {
            for (auto input = G_UnsortedData.begin(), output = G_SortedData.begin();
            input != G_UnsortedData.end(); ++input, ++output)
            {
                LAI_IMSET iset(input->begin(), input->end());
                AssertContainerEqual(iset, *output);
            }

            for (auto input = G_UnsortedData.begin(), output = G_ReverseSortedData.begin();
            input != G_UnsortedData.end(); ++input, ++output)
            {
                lai::multiset<int, std::greater<>> iset(input->begin(), input->end(), std::greater<>());
                AssertContainerEqual(iset, *output);
            }
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            for (auto input = G_UnsortedData.begin(), output = G_SortedData.begin();
            input != G_UnsortedData.end(); ++input, ++output)
            {
                LAI_IMSET iset = *input;
                AssertContainerEqual(iset, *output);
            }

            for (auto input = G_UnsortedData.begin(), output = G_ReverseSortedData.begin();
            input != G_UnsortedData.end(); ++input, ++output)
            {
                lai::multiset<int, std::greater<>> iset(*input, std::greater<>());
                AssertContainerEqual(iset, *output);
            }
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::multiset);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::multiset);
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::multiset);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::multiset);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::multiset);

            LAI_SMSET s;

            s = {};
            AssertContainerEqual(s, S_IL{});

            s = { "" };
            AssertContainerEqual(s, S_IL{ "" });

            s = { "", "" };
            AssertContainerEqual(s, S_IL{ "", "" });

            s = { "", "", "" };
            AssertContainerEqual(s, S_IL{ "", "", "" });

            s = { "b", "", "a", "", "c", "", "z", "a" };
            AssertContainerEqual(s, S_IL{ "", "", "", "a", "a", "b", "c", "z" });
        }

        TEST_METHOD(TestInsertLvalue)
        {
            S_IL data = { "a", "a", "a", "", "", "", "b", "b", "b", "", "a", "b" };

            LAI_SMSET s;
            std::multiset<std::string> stdS;

            for (auto & str : data)
            {
                auto iter = s.insert(str);
                IS_TRUE(*iter == str);

                stdS.insert(str);
                AssertContainerEqual(s, stdS);
            }
        }

        TEST_METHOD(TestInsertRvalue)
        {
            I_IL data = { 2,2,2,-1,-1,-1,4,4,4,-1,2,4 };
            
            LAI_UMSET s;
            std::multiset<Uncopyable> stdS;

            for (auto & i : data)
            {
                auto iter = s.insert(Uncopyable(i));
                IS_TRUE(*iter == Uncopyable(i));

                stdS.insert(Uncopyable(i));
                AssertContainerEqual(s, stdS);
            }
        }

        TEST_METHOD(TestInsertHintLvalue)
        {
            LAI_SMSET s;
            std::string str;

            str = "";
            auto iter = s.insert(s.begin(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "" });

            str = "m";
            iter = s.insert(s.begin(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "m" });

            str = "";
            iter = s.insert(s.begin(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "m" });

            str = "m";
            iter = s.insert(s.end(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "m", "m" });

            str = "g";
            iter = s.insert(s.end(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "g", "m", "m" });

            str = "g";
            iter = s.insert(std::next(s.begin(), 2), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "g", "g", "m", "m" });

            str = "c";
            iter = s.insert(std::next(s.begin(), 2), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "c", "g", "g", "m", "m" });

            str = "h";
            iter = s.insert(std::next(s.begin(), 2), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "", "", "c", "g", "g", "h", "m", "m" });
        }


        template<typename TSet, typename TStdSet>
        void TestInsertHintRvalueHelper(TSet & s, TStdSet & stdS, int value, int off)
        {
            auto iter = s.insert(std::next(s.begin(), off), Uncopyable(value));
            IS_TRUE(*iter == Uncopyable(value));

            stdS.insert(std::next(stdS.begin(), off), Uncopyable(value));
            AssertContainerEqual(s, stdS);
        }

        TEST_METHOD(TestInsertHintRvalue)
        {
            LAI_UMSET s;
            std::multiset<Uncopyable> stdS;

            TestInsertHintRvalueHelper(s, stdS, 0, 0);

            TestInsertHintRvalueHelper(s, stdS, 10, 0);

            TestInsertHintRvalueHelper(s, stdS, 0, 0);

            TestInsertHintRvalueHelper(s, stdS, 10, 0);

            TestInsertHintRvalueHelper(s, stdS, 5, static_cast<int>(s.size()));

            TestInsertHintRvalueHelper(s, stdS, 5, 2);

            TestInsertHintRvalueHelper(s, stdS, 3, 2);

            TestInsertHintRvalueHelper(s, stdS, 7, 2);
        }

        TEST_METHOD(TestInsertInitList)
        {
            LAI_IMSET s;

            s.insert({});
            AssertContainerEqual(s, I_IL{});

            s.insert({ 1 });
            AssertContainerEqual(s, I_IL{ 1 });

            s.insert({ 1, 1 });
            AssertContainerEqual(s, I_IL{ 1,1,1 });

            s.insert({ 0,1,2 });
            AssertContainerEqual(s, I_IL{ 0,1,1,1,1,2 });
        }

        TEST_METHOD(TestInsertRange)
        {
            LAI_IMSET s;

            I_IL data = {};
            s.insert(data.begin(), data.end());
            AssertContainerEqual(s, I_IL{});

            data = { 1 };
            s.insert(data.begin(), data.end());
            AssertContainerEqual(s, I_IL{ 1 });

            data = { 1,1 };
            s.insert(data.begin(), data.end());
            AssertContainerEqual(s, I_IL{ 1,1,1 });

            data = { 0,1,2 };
            s.insert(data.begin(), data.end());
            AssertContainerEqual(s, I_IL{ 0,1,1,1,1,2 });
        }

        TEST_METHOD(TestEmplace)
        {
            LAI_SMSET s;

            auto iter = s.emplace();
            IS_TRUE(*iter == "");
            AssertContainerEqual(s, S_IL{ "" });

            iter = s.emplace();
            IS_TRUE(*iter == "");
            AssertContainerEqual(s, S_IL{ "", "" });

            iter = s.emplace(1, 'k');
            IS_TRUE(*iter == "k");
            AssertContainerEqual(s, S_IL{ "", "", "k" });

            iter = s.emplace("z");
            IS_TRUE(*iter == "z");
            AssertContainerEqual(s, S_IL{ "", "", "k", "z" });

            iter = s.emplace(std::initializer_list<char>{ 'k' });
            IS_TRUE(*iter == "k");
            AssertContainerEqual(s, S_IL{ "", "", "k", "k", "z" });
        }

        TEST_METHOD(TestEmplaceHint)
        {
            LAI_SMSET s;

            auto iter = s.emplace_hint(s.begin());
            IS_TRUE(*iter == "");
            AssertContainerEqual(s, S_IL{ "" });

            iter = s.emplace_hint(s.end());
            IS_TRUE(*iter == "");
            AssertContainerEqual(s, S_IL{ "", "" });

            iter = s.emplace_hint(s.end(), 1, 'k');
            IS_TRUE(*iter == "k");
            AssertContainerEqual(s, S_IL{ "", "", "k" });

            iter = s.emplace_hint(std::next(s.begin(), 2), "z");
            IS_TRUE(*iter == "z");
            AssertContainerEqual(s, S_IL{ "", "", "k", "z" });

            iter = s.emplace_hint(--s.end(), std::initializer_list<char>{ 'k' });
            IS_TRUE(*iter == "k");
            AssertContainerEqual(s, S_IL{ "", "", "k", "k", "z" });
        }

        TEST_METHOD(TestEraseKeys)
        {
            LAI_IMSET s = { 0,2,2,4,4,4,7,7,7,9,11 };

            IS_TRUE(s.erase(-1) == 0);
            AssertContainerEqual(s, I_IL{ 0,2,2,4,4,4,7,7,7,9,11 });

            IS_TRUE(s.erase(12) == 0);
            AssertContainerEqual(s, I_IL{ 0,2,2,4,4,4,7,7,7,9,11 });

            IS_TRUE(s.erase(2) == 2);
            AssertContainerEqual(s, I_IL{ 0,4,4,4,7,7,7,9,11 });

            IS_TRUE(s.erase(5) == 0);
            AssertContainerEqual(s, I_IL{ 0,4,4,4,7,7,7,9,11 });

            IS_TRUE(s.erase(7) == 3);
            AssertContainerEqual(s, I_IL{ 0,4,4,4,9,11 });

            IS_TRUE(s.erase(9) == 1);
            AssertContainerEqual(s, I_IL{ 0,4,4,4,11 });

            IS_TRUE(s.erase(0) == 1);
            AssertContainerEqual(s, I_IL{ 4,4,4,11 });

            IS_TRUE(s.erase(11) == 1);
            AssertContainerEqual(s, I_IL{ 4,4,4 });

            IS_TRUE(s.erase(4) == 3);
            AssertContainerEqual(s, I_IL{});

            IS_TRUE(s.erase(0) == 0);
            AssertContainerEqual(s, I_IL{});
        }

        TEST_METHOD(TestCount)
        {
            LAI_IMSET s = { 0,2,2,4,4,4,7,7,7,9,11 };

            IS_TRUE(s.count(-1) == 0);
            IS_TRUE(s.count(12) == 0);

            IS_TRUE(s.count(0) == 1);
            IS_TRUE(s.count(1) == 0);
            IS_TRUE(s.count(2) == 2);
            IS_TRUE(s.count(3) == 0);
            IS_TRUE(s.count(4) == 3);
            IS_TRUE(s.count(5) == 0);
            IS_TRUE(s.count(7) == 3);
            IS_TRUE(s.count(9) == 1);
            IS_TRUE(s.count(11) == 1);
        }

        TEST_METHOD(TestFind)
        {
            LAI_IMSET s = { 0,2,2,4,4,4,7,7,7,9,11 };

            IS_TRUE(s.find(-1) == s.end());
            IS_TRUE(s.find(12) == s.end());

            IS_TRUE(s.find(0) == s.begin());
            IS_TRUE(s.find(1) == s.end());
            IS_TRUE(s.find(2) == ++s.begin());
            IS_TRUE(s.find(3) == s.end());
            IS_TRUE(s.find(4) == std::next(s.begin(), 3));
            IS_TRUE(s.find(5) == s.end());
            IS_TRUE(s.find(7) == std::next(s.begin(), 6));
            IS_TRUE(s.find(9) == std::next(s.end(), -2));
            IS_TRUE(s.find(11) == --s.end());
        }

        TEST_METHOD(TestBounds)
        {
            LAI_IMSET s = { 1,3,3,6,9,9,9,10 };

            IS_TRUE(s.lower_bound(0) == s.begin());
            IS_TRUE(s.lower_bound(1) == s.begin());
            IS_TRUE(s.lower_bound(2) == ++s.begin());
            IS_TRUE(s.lower_bound(3) == ++s.begin());
            IS_TRUE(s.lower_bound(4) == std::next(s.begin(), 3));
            IS_TRUE(s.lower_bound(9) == std::next(s.end(), -4));
            IS_TRUE(s.lower_bound(10) == --s.end());
            IS_TRUE(s.lower_bound(11) == s.end());

            IS_TRUE(s.upper_bound(0) == s.begin());
            IS_TRUE(s.upper_bound(1) == ++s.begin());
            IS_TRUE(s.upper_bound(2) == ++s.begin());
            IS_TRUE(s.upper_bound(3) == std::next(s.begin(), 3));
            IS_TRUE(s.upper_bound(4) == std::next(s.begin(), 3));
            IS_TRUE(s.upper_bound(9) == --s.end());
            IS_TRUE(s.upper_bound(10) == s.end());
            IS_TRUE(s.upper_bound(11) == s.end());

            auto range = s.equal_range(0);
            IS_TRUE(range.first == s.begin());
            IS_TRUE(range.second == s.begin());

            range = s.equal_range(1);
            IS_TRUE(range.first == s.begin());
            IS_TRUE(range.second == ++s.begin());

            range = s.equal_range(2);
            IS_TRUE(range.first == ++s.begin());
            IS_TRUE(range.second == ++s.begin());

            range = s.equal_range(3);
            IS_TRUE(range.first == ++s.begin());
            IS_TRUE(range.second == std::next(s.begin(), 3));

            range = s.equal_range(9);
            IS_TRUE(range.first == std::next(s.begin(), 4));
            IS_TRUE(range.second == --s.end());

            range = s.equal_range(10);
            IS_TRUE(range.first == --s.end());
            IS_TRUE(range.second == s.end());

            range = s.equal_range(11);
            IS_TRUE(range.first == s.end());
            IS_TRUE(range.second == s.end());
        }

        TEST_METHOD(TestMemberSwap)
        {
            TSC_MemberSwap(lai::multiset);
        }

        TEST_METHOD(TestNonMemberSwap)
        {
            TSC_NonMemberSwap(lai::multiset, lai::swap);
        }

        TEST_METHOD(TestRelationalOperator)
        {
            TSC_RelationalOperator(lai::multiset);
        }
    };
}