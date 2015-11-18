#include "stdafx.h"
#include "../LaiSTL/Set.h"
#include <set>
#include <functional>

#define LAI_ISET lai::set<int>
#define LAI_SSET lai::set<std::string>
#define LAI_USET lai::set<Uncopyable>

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

        std::initializer_list<I_IL> G_UniqueSortedData =
        {
            {},
            { 0 },
            { 0 },
            { 0 },
            { 0, 1 },
            { 0, 1 },
            { 0, 1, 2 },
            { 0, 1, 2 },
            { 0, 1, 2 },
            { 0, 1, 2 },
            { 0, 1, 2 },
            { 0, 1, 2 },
            { 0, 1 },
            { 0, 1 },
            { 0, 1 },
            { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
            { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
            { -11, -3, 9, 17, 42, 54, 602, 999 },
            { -11, -3, 9, 17, 42, 54, 602, 999 },
        };

        std::initializer_list<I_IL> G_UniqueReverseSortedData =
        {
            {},
            { 0 },
            { 0 },
            { 0 },
            { 1, 0 },
            { 1, 0 },
            { 2, 1, 0 },
            { 2, 1, 0 },
            { 2, 1, 0 },
            { 2, 1, 0 },
            { 2, 1, 0 },
            { 2, 1, 0 },
            { 1, 0 },
            { 1, 0 },
            { 1, 0 },
            { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
            { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
            { 999, 602, 54, 42, 17, 9, -3, -11 },
            { 999, 602, 54, 42, 17, 9, -3, -11 },
        };
    }

    TEST_CLASS(TestSet)
    {
        template<typename TSet>
        void TestCtorHelper(TSet & set)
        {
            IS_TRUE(set.empty());
            IS_TRUE(set.size() == 0);
            IS_TRUE(set.begin() == set.end());
            IS_TRUE(set.cbegin() == set.cend());
            IS_TRUE(set.rbegin() == set.rend());
            IS_TRUE(set.crbegin() == set.crend());
        }

        TEST_METHOD(TestDefaultConstructor)
        {
            TSC_DefaultConstructor(lai::set);

            auto greater = std::greater<>();
            TSC_DefaultConstructorAux<lai::set<int, std::greater<>>>();
            TSC_DefaultConstructorAux<lai::set<std::string, std::greater<>>>();
            TSC_DefaultConstructorAux<lai::set<Uncopyable, std::greater<>>>();
        }

        TEST_METHOD(TestRangeConstructor)
        {
            for (auto input = G_UnsortedData.begin(), output = G_UniqueSortedData.begin();
                    input != G_UnsortedData.end(); ++input, ++output)
            {
                LAI_ISET iset(input->begin(), input->end());
                AssertContainerEqual(iset, *output);
            }

            for (auto input = G_UnsortedData.begin(), output = G_UniqueReverseSortedData.begin();
                    input != G_UnsortedData.end(); ++input, ++output)
            {
                lai::set<int, std::greater<>> iset(input->begin(), input->end(), std::greater<>());
                AssertContainerEqual(iset, *output);
            }
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            for (auto input = G_UnsortedData.begin(), output = G_UniqueSortedData.begin();
                input != G_UnsortedData.end(); ++input, ++output)
            {
                LAI_ISET iset = *input;
                AssertContainerEqual(iset, *output);
            }

            for (auto input = G_UnsortedData.begin(), output = G_UniqueReverseSortedData.begin();
                    input != G_UnsortedData.end(); ++input, ++output)
            {
                lai::set<int, std::greater<>> iset(*input, std::greater<>());
                AssertContainerEqual(iset, *output);
            }
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::set);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::set);
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::set);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::set);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::set);

            LAI_ISET s;
            s = { 1,1 };
            AssertContainerEqual(s, I_IL{ 1 });

            s = { 1,1,1 };
            AssertContainerEqual(s, I_IL{ 1 });

            s = { 2,3,0,0,1,1,4 };
            AssertContainerEqual(s, I_IL{ 0,1,2,3,4 });
        }

        TEST_METHOD(TestInsertLvalue)
        {
            S_IL data =
            {
                "z", "k", "z", "u", "k", "u", "a", "zz"
            };

            std::initializer_list<std::pair<std::string, bool>> result =
            {
                {"z", true}, {"k", true},
                {"z", false}, {"u", true},
                {"k", false}, {"u", false},
                {"a", true}, {"zz", true}
            };

            S_IL finalResult = { "a", "k", "u", "z", "zz" };

            LAI_SSET s;
            auto input = data.begin();
            auto output = result.begin();
            while (input != data.end())
            {
                auto ret = s.insert(*input);
                IS_TRUE(*ret.first == output->first);
                IS_TRUE(ret.second == output->second);
                ++output;
                ++input;
            }
            AssertContainerEqual(s, finalResult);
        }

        TEST_METHOD(TestInsertRvalue)
        {
            I_IL data =
            {
                9, 5, 9, 7, 5, 7, 0, 10
            };

            std::initializer_list<std::pair<int, bool>> result =
            {
                { 9, true },{ 5, true },
                { 9, false },{ 7, true },
                { 5, false },{ 7, false },
                { 0, true },{ 10, true }
            };


            LAI_USET s;
            auto input = data.begin();
            auto output = result.begin();
            while (input != data.end())
            {
                auto ret = s.insert(Uncopyable(*input));
                IS_TRUE(*ret.first == Uncopyable(output->first));
                IS_TRUE(ret.second == output->second);
                ++output;
                ++input;
            }

            STD_UVEC finalResult;
            finalResult.push_back(Uncopyable(0));
            finalResult.push_back(Uncopyable(5));
            finalResult.push_back(Uncopyable(7));
            finalResult.push_back(Uncopyable(9));
            finalResult.push_back(Uncopyable(10));
            AssertContainerEqual(s, finalResult);
        }

        TEST_METHOD(TestInsertHintLvalue)
        {
            LAI_SSET s;
            std::string str;

            str = "k";
            auto iter = s.insert(s.begin(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ str });

            iter = s.insert(s.begin(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ str });

            iter = s.insert(s.end(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ str });

            str = "z";
            iter = s.insert(s.end(), str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "k", "z" });

            str = "u";
            iter = s.insert(iter, str);
            IS_TRUE(*iter == str);
            AssertContainerEqual(s, S_IL{ "k", "u", "z" });
        }

        TEST_METHOD(TestInsertHintRvalue)
        {
            LAI_USET s;
            
            auto iter = s.insert(s.begin(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.insert(s.begin(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.insert(s.end(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.insert(s.end(), Uncopyable(10));
            IS_TRUE(*iter == Uncopyable(10));
            IS_TRUE(s.size() == 2);

            iter = s.insert(iter, Uncopyable(5));
            IS_TRUE(*iter == Uncopyable(5));
            IS_TRUE(s.size() == 3);
        }

        TEST_METHOD(TestInsertRange)
        {
            LAI_SSET s;
            S_IL data = { };

            s.insert(data.begin(), data.end());
            AssertContainerEqual(s, data);

            S_IL data1 = { "u" };
            s.insert(data1.begin(), data1.end());
            AssertContainerEqual(s, data1);

            S_IL data2 = { "z", "k", "z", "u", "k", "u", "a", "zz" };
            s.insert(data2.begin(), data2.end());
            AssertContainerEqual(s, S_IL{ "a", "k", "u", "z", "zz" });
        }

        TEST_METHOD(TestInsertInitList)
        {
            LAI_SSET s;

            s.insert({});
            AssertContainerEqual(s, S_IL{});

            s.insert({ "u" });
            AssertContainerEqual(s, S_IL{ "u" });

            s.insert({ "z", "k", "z", "u", "k", "u", "a", "zz" });
            AssertContainerEqual(s, S_IL{ "a", "k", "u", "z", "zz" });
        }

        TEST_METHOD(TestEmplace)
        {
            LAI_SSET s;

            auto ret = s.emplace();
            IS_TRUE(*ret.first == "");
            IS_TRUE(ret.second == true);
            AssertContainerEqual(s, S_IL{ "" });

            ret = s.emplace();
            IS_TRUE(*ret.first == "");
            IS_TRUE(ret.second == false);
            AssertContainerEqual(s, S_IL{ "" });

            ret = s.emplace(1, 'k');
            IS_TRUE(*ret.first == "k");
            IS_TRUE(ret.second == true);
            AssertContainerEqual(s, S_IL{ "", "k" });

            ret = s.emplace("z");
            IS_TRUE(*ret.first == "z");
            IS_TRUE(ret.second == true);
            AssertContainerEqual(s, S_IL{ "", "k", "z" });

            ret = s.emplace(std::initializer_list<char>{ 'k' });
            IS_TRUE(*ret.first == "k");
            IS_TRUE(ret.second == false);
            AssertContainerEqual(s, S_IL{ "", "k", "z" });
        }

        TEST_METHOD(TestEmplaceHint)
        {
            LAI_USET s;

            auto iter = s.emplace_hint(s.begin(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.emplace_hint(s.begin(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.emplace_hint(s.end(), Uncopyable(3));
            IS_TRUE(*iter == Uncopyable(3));
            IS_TRUE(s.size() == 1);

            iter = s.emplace_hint(s.end(), Uncopyable(10));
            IS_TRUE(*iter == Uncopyable(10));
            IS_TRUE(s.size() == 2);

            iter = s.emplace_hint(iter, Uncopyable(5));
            IS_TRUE(*iter == Uncopyable(5));
            IS_TRUE(s.size() == 3);
        }

        TEST_METHOD(TestErase)
        {
            LAI_SSET s = { "a" };

            auto iter = s.erase(s.begin());
            IS_TRUE(iter == s.begin());
            IS_TRUE(s.empty());

            s = { "a", "b" };
            iter = s.erase(s.begin());
            IS_TRUE(iter == s.begin());
            AssertContainerEqual(s, S_IL{ "b" });

            s = { "b", "a" };
            iter = s.erase(++s.begin());
            IS_TRUE(iter == s.end());
            AssertContainerEqual(s, S_IL{ "a" });

            s = { "a", "b", "c" };
            iter = s.erase(s.begin());
            IS_TRUE(iter == s.begin());
            AssertContainerEqual(s, S_IL{ "b", "c" });

            s = { "k", "g", "u", "o", "z" };
            iter = s.erase(++s.begin());
            IS_TRUE(iter == ++s.begin());
            AssertContainerEqual(s, S_IL{ "g", "o", "u", "z" });
        }

        TEST_METHOD(TestEraseRange)
        {
            LAI_SSET s;

            auto iter = s.erase(s.begin(), s.end());
            IS_TRUE(iter == s.begin());
            AssertContainerEqual(s, S_IL{});

            s = { "a" };
            iter = s.erase(s.begin(), s.begin());
            IS_TRUE(iter == s.begin());
            AssertContainerEqual(s, S_IL{ "a" });

            iter = s.erase(s.begin(), s.end());
            IS_TRUE(iter == s.end());
            AssertContainerEqual(s, S_IL{});

            s = { "a", "b" };
            iter = s.erase(s.begin(), ++s.begin());
            IS_TRUE(iter == s.begin());
            AssertContainerEqual(s, S_IL{ "b" });

            s = { "a", "b", "c" };
            iter = s.erase(++s.begin(), s.end());
            IS_TRUE(iter == ++s.begin());
            AssertContainerEqual(s, S_IL{ "a" });
        }

        TEST_METHOD(TestEraseKey)
        {
            LAI_SSET s;

            LAI_SSET::size_type erasedCount = s.erase("");
            IS_TRUE(erasedCount == 0);
            IS_TRUE(s.empty());

            s = { "a" };
            erasedCount = s.erase("");
            IS_TRUE(erasedCount == 0);
            AssertContainerEqual(s, S_IL{ "a" });

            erasedCount = s.erase("a");
            IS_TRUE(erasedCount == 1);
            IS_TRUE(s.empty());

            s = { "a", "d", "g", "l", "o", "z" };
            erasedCount = s.erase("a");
            IS_TRUE(erasedCount == 1);
            AssertContainerEqual(s, S_IL{ "d", "g", "l", "o", "z" });

            erasedCount = s.erase("f");
            IS_TRUE(erasedCount == 0);
            AssertContainerEqual(s, S_IL{ "d", "g", "l", "o", "z" });

            erasedCount = s.erase("l");
            IS_TRUE(erasedCount == 1);
            AssertContainerEqual(s, S_IL{ "d", "g", "o", "z" });

            erasedCount = s.erase("z");
            IS_TRUE(erasedCount == 1);
            AssertContainerEqual(s, S_IL{ "d", "g", "o" });
        }

        TEST_METHOD(TestMemberSwap)
        {
            TSC_MemberSwap(lai::set);
        }

        TEST_METHOD(TestNonMemberSwap)
        {
            TSC_NonMemberSwap(lai::set, lai::swap);
        }

        TEST_METHOD(TestBounds)
        {
            // from stlport : http://www.stlport.org/

            LAI_ISET s{ 1, 3, 6, 7 };
            const LAI_ISET & crs = s;

            LAI_ISET::iterator sit;
            LAI_ISET::const_iterator scit;
            std::pair<LAI_ISET::iterator, LAI_ISET::iterator> pit;
            std::pair<LAI_ISET::const_iterator, LAI_ISET::const_iterator> pcit;

            //Check iterator on mutable set
            sit = s.lower_bound(2);
            IS_TRUE(sit != s.end());
            IS_TRUE(*sit == 3);

            sit = s.upper_bound(5);
            IS_TRUE(sit != s.end());
            IS_TRUE(*sit == 6);

            pit = s.equal_range(6);
            IS_TRUE(pit.first != pit.second);
            IS_TRUE(pit.first != s.end());
            IS_TRUE(*pit.first == 6);
            IS_TRUE(pit.second != s.end());
            IS_TRUE(*pit.second == 7);

            pit = s.equal_range(4);
            IS_TRUE(pit.first == pit.second);
            IS_TRUE(pit.first != s.end());
            IS_TRUE(*pit.first == 6);
            IS_TRUE(pit.second != s.end());
            IS_TRUE(*pit.second == 6);

            //Check const_iterator on mutable set
            scit = s.lower_bound(2);
            IS_TRUE(scit != s.end());
            IS_TRUE(*scit == 3);

            scit = s.upper_bound(5);
            IS_TRUE(scit != s.end());
            IS_TRUE(*scit == 6);

            pcit = s.equal_range(6);
            IS_TRUE(pcit.first != pcit.second);
            IS_TRUE(pcit.first != s.end());
            IS_TRUE(*pcit.first == 6);
            IS_TRUE(pcit.second != s.end());
            IS_TRUE(*pcit.second == 7);

            //Check const_iterator on const set
            scit = crs.lower_bound(2);
            IS_TRUE(scit != crs.end());
            IS_TRUE(*scit == 3);

            scit = crs.upper_bound(5);
            IS_TRUE(scit != crs.end());
            IS_TRUE(*scit == 6);

            pcit = crs.equal_range(6);
            IS_TRUE(pcit.first != pcit.second);
            IS_TRUE(pcit.first != crs.end());
            IS_TRUE(*pcit.first == 6);
            IS_TRUE(pcit.second != crs.end());
            IS_TRUE(*pcit.second == 7);
        }

        TEST_METHOD(TestCount)
        {
            LAI_SSET s;
            IS_TRUE(s.count("") == 0);

            s = { "a", "d", "g", "l", "o", "z" };
            IS_TRUE(s.count("") == 0);
            IS_TRUE(s.count("a") == 1);
            IS_TRUE(s.count("c") == 0);
            IS_TRUE(s.count("l") == 1);
            IS_TRUE(s.count("m") == 0);
            IS_TRUE(s.count("o") == 1);
            IS_TRUE(s.count("z") == 1);
        }

        TEST_METHOD(TestFind)
        {
            LAI_SSET s;
            IS_TRUE(s.find("") == s.end());

            s = { "a", "d", "g", "l", "o", "z" };
            IS_TRUE(s.find("") == s.end());
            IS_TRUE(s.find("a") == s.begin());
            IS_TRUE(s.find("c") == s.end());
            IS_TRUE(s.find("l") == std::next(s.begin(), 3));
            IS_TRUE(s.find("m") == s.end());
            IS_TRUE(s.find("o") == std::next(s.begin(), 4));
            IS_TRUE(s.find("z") == --s.end());

            const LAI_SSET ccs;
            IS_TRUE(ccs.find("") == ccs.end());

            const LAI_SSET cs = { "a", "d", "g", "l", "o", "z" };
            IS_TRUE(cs.find("") == cs.end());
            IS_TRUE(cs.find("a") == cs.begin());
            IS_TRUE(cs.find("c") == cs.end());
            IS_TRUE(cs.find("l") == std::next(cs.begin(), 3));
            IS_TRUE(cs.find("m") == cs.end());
            IS_TRUE(cs.find("o") == std::next(cs.begin(), 4));
            IS_TRUE(cs.find("z") == --cs.end());
        }

        TEST_METHOD(TestRelationalOperator)
        {
            TSC_RelationalOperator(lai::set);
        }
    };
}