#include "stdafx.h"
#include "../LaiSTL/Vector.h"
#include <memory>
#include <vector>
#include <string>

// following four macros undefined at end of this file.

#define LAI_IVEC lai::vector<int>
#define LAI_SVEC lai::vector<std::string>
#define STD_IVEC std::vector<int>
#define STD_SVEC std::vector<std::string>

namespace UnitTest
{
    TEST_CLASS(TestVector)
    {
    public:
        using unCopyable = std::unique_ptr<int>;

    public:
        TEST_METHOD(TestCapacityOperations)
        {
            int size = 3;
            lai::vector<unCopyable> vec(size);

            IS_FALSE(vec.empty());
            IS_TRUE(vec.size() == size);
            IS_TRUE(vec.capacity() == size);

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

        TEST_METHOD(TestCountConstructor)
        {
            int count = 10;
            int initVal = 42;

            LAI_IVEC vec1(count, initVal);
            IS_TRUE(vec1.size() == count);
            AssertContainerFilled(vec1, initVal);

            lai::vector<unCopyable> vec2(count);
            IS_TRUE(vec2.size() == count);
            for (auto & i : vec2)
            {
                IS_TRUE(i == unCopyable());
            }

            LAI_IVEC vec3(0);
            IS_TRUE(vec3.size() == 0);
        }

        TEST_METHOD(TestRangeContructor)
        {
            STD_IVEC stdV{ 0,1,2,3,4,5 };
            LAI_IVEC laiV(stdV.begin(), stdV.end());
            AssertContainerEqual(laiV, stdV);

            LAI_IVEC laiV2(laiV.begin(), laiV.end());
            AssertContainerEqual(laiV, laiV2);
            AssertContainerEqual(laiV2, stdV);
        }

        TEST_METHOD(TestBracedListConstructor)
        {
            LAI_IVEC v{ 10,20 };
            AssertContainerEqual(v, STD_IVEC{10, 20});

            LAI_IVEC v1{ 0,1,2 };
            AssertContainerEqual(v1, STD_IVEC{ 0, 1, 2 });

            LAI_IVEC v2{ 10 };
            AssertContainerEqual(v2, STD_IVEC{10});

            LAI_SVEC v3{ 10, "a" };
            AssertContainerEqual(v3, STD_SVEC{10, "a"});

        }

        TEST_METHOD(TestAt)
        {
            LAI_IVEC v{ 7 };
            IS_TRUE(v[0] == 7);
            Assert::ExpectException<std::out_of_range>([&v] { v.at(1); });
            Assert::ExpectException<std::out_of_range>([&v] { v.at(10); });
            Assert::ExpectException<std::out_of_range>([&v] { v.at(-1); });
            Assert::ExpectException<std::out_of_range>([&v] { v.at(-10); });
        }

        TEST_METHOD(TestCopyConstructor)
        {
            LAI_IVEC v{ 0,1,2,3,4,5 };
            LAI_IVEC v1(v);

            IS_TRUE(v.size() == v1.size() && v1.size() == 6);
            AssertContainerEqual(v, v1);

            v.~vector();
            IS_TRUE(v1.size() == 6);
            AssertContainerEqual(v1, STD_IVEC{0, 1, 2, 3, 4, 5});
        }

        TEST_METHOD(TestMoveConstructor)
        {
            lai::vector<unCopyable> v(3);
            std::vector<unCopyable> stdV(3);
            lai::vector<unCopyable> movedV(std::move(v));

            IS_TRUE(movedV.size() == 3);
            AssertContainerEqual(movedV, stdV);

            v.~vector();
            IS_TRUE(movedV.size() == 3);
            AssertContainerEqual(movedV, stdV);
        }

        TEST_METHOD(TestPushBack)
        {
            lai::vector<unCopyable> v;
            v.push_back(std::move(unCopyable()));
            v.push_back(unCopyable());
            AssertContainerEqual(v, std::vector<unCopyable>(2));

            LAI_IVEC v1{ 1, 2 };
            v1.push_back(3);
            v1.push_back(4);
            AssertContainerEqual(v1, STD_IVEC{1, 2, 3, 4});

            LAI_SVEC v2{ "l", "a", "i" };
            v2.push_back(v2[1]);
            v2.push_back(std::move(v2[0]));
            IS_TRUE(v2[4] == "l");
            IS_TRUE(v2[3] == "a");
        }

        TEST_METHOD(TestEmplaceBack)
        {
            std::string s("a");
            LAI_SVEC v;
            v.emplace_back(1, 'a');
            v.emplace_back("a");
            v.emplace_back(s);
            v.emplace_back(std::move(s));
            AssertContainerEqual(v, STD_SVEC{4, "a"});

            lai::vector<unCopyable> v1;
            v1.emplace_back(unCopyable(new int(3)));
            v1.emplace_back(std::make_unique<int>(3));
            IS_TRUE(*(v1[0]) == 3);
            IS_TRUE(*(v1[1]) == 3);
        }


        TEST_METHOD(TestPopBack)
        {
            LAI_SVEC v{ "a", "b", "c" };
            v.pop_back();
            AssertContainerEqual(v, STD_SVEC{"a", "b"});
            v.pop_back();
            AssertContainerEqual(v, STD_SVEC{"a"});
            v.pop_back();
            AssertContainerEqual(v, STD_SVEC{});
        }


        TEST_METHOD(TestOperatorAssign)
        {
            std::initializer_list<std::string> datas = { "l", "a", "i", "s", "t", "l" };
            LAI_SVEC v(datas);
            LAI_SVEC v1, v2;
            v1 = v;
            v2 = v1;
            AssertContainerEqual(v, v1);
            AssertContainerEqual(v, v2);
            AssertContainerEqual(v1, STD_SVEC{datas});

            v1.clear();
            v1 = std::move(v);
            v.~vector();
            AssertContainerEqual(v1, STD_SVEC{datas});

            v1.clear();
            v1 = datas;
            AssertContainerEqual(v1, STD_SVEC{datas});
   
            v1.clear();
            v1 = { 10, "a" };
            AssertContainerEqual(v1, STD_SVEC{10, "a"});
        }


        TEST_METHOD(TestErase)
        {
            std::initializer_list<std::string> datas = { "l", "a", "i", "s", "t", "l" };
            STD_SVEC v(datas);

            auto it = v.erase(v.begin());   // v == aistl
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_SVEC{"a", "i", "s", "t", "l"});


            it = v.erase(v.begin() + 1);    // v == astl
            IS_TRUE(it == v.begin() + 1);
            AssertContainerEqual(v, STD_SVEC{"a", "s", "t", "l"});


            it = v.erase(v.begin(), v.begin() + 2);     // v == tl
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_SVEC{"t", "l"});

            it = v.erase(v.begin(), v.begin());     // v == tl
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_SVEC{"t", "l"});

            v = datas;      // v == laistl
            it = v.erase(v.begin() + 1, v.begin() + 4);     // v == ltl
            IS_TRUE(it == v.begin() + 1);
            AssertContainerEqual(v, STD_SVEC{ "l", "t", "l" });

            it = v.erase(v.begin(), v.end());
            IS_TRUE(it == v.begin());
            IS_TRUE(it == v.end());
        }

        TEST_METHOD(TestResizeA)
        {
            // test resize()

            std::initializer_list<std::string> datas = { "l", "a", "i", "s", "t", "l" };
            LAI_SVEC v(datas);

            v.resize(v.size());
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "s", "t", "l" });

            v.resize(v.size() - 1);     // v == laist
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "s", "t" });

            v.resize(v.size() - 2);     // v == lai
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i" });

            v.resize(v.size() + 1);     // v == lai.
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "" });

            v.resize(v.size() + 2);     // v == lai...
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "", "", "" });


            lai::vector<unCopyable> v1;
            v1.resize(10);
            IS_TRUE(v1.size() == 10);
        }

        TEST_METHOD(TestResizeB)
        {
            // test resize( , )

            std::initializer_list<std::string> datas = { "l", "a", "i", "s", "t", "l" };
            LAI_SVEC v(datas);

            v.resize(v.size(), "");
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "s", "t", "l" });

            v.resize(v.size() - 1, "a");     // v == laist
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "s", "t" });

            v.resize(v.size() - 2, "b");     // v == lai
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i" });

            v.resize(v.size() + 1, "c");     // v == laic
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "c" });

            v.resize(v.size() + 2, "d");     // v == laicdd
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "c", "d", "d" });
        }


        TEST_METHOD(TestEmplace)
        {
            LAI_IVEC v{ 1,2,3 };

            auto it = v.emplace(v.begin(), 0);
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3 });

            it = v.emplace(v.end(), 5);
            IS_TRUE(it == v.end() - 1);
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3,5 });

            it = v.emplace(v.end() - 1, 4);
            IS_TRUE(it == v.end() - 2);
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3,4,5 });


            LAI_SVEC sv{ "stl" };

            auto sit = sv.emplace(sv.begin(), 3, 'a');
            IS_TRUE(sit == sv.begin());
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "stl" });

            sit = sv.emplace(sv.end(), "bbb");
            IS_TRUE(sit == sv.end() - 1);
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "stl", "bbb" });


            sit = sv.emplace(sv.begin() + 1);
            IS_TRUE(sit == sv.begin() + 1);
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "", "stl", "bbb" });

            sv.clear();
            sit = sv.emplace(sv.emplace(sv.emplace(sv.begin(), "i"), "a"), "l");
            IS_TRUE(sit == sv.begin());
            AssertContainerEqual(sv, STD_SVEC{ "l", "a","i" });


            lai::vector<unCopyable> uv;
            for (int i = 0; i != 100; ++i)
            {
                uv.emplace(uv.begin(), unCopyable());
            }
            AssertContainerEqual(uv, std::vector<unCopyable>(100));
        }

        TEST_METHOD(TestDefaultConstructor)
        {
            LAI_IVEC v;
            IS_FALSE(v.begin());
            IS_FALSE(v.end());
            IS_TRUE(v.begin() == v.end());
            IS_TRUE(v.empty());
            IS_TRUE(v.size() == 0);
            Assert::ExpectException<std::out_of_range>([&v] { v.at(0); });
        }

        TEST_METHOD(TestInsert)
        {
            LAI_IVEC v{ 1,2,3 };

            auto it = v.insert(v.begin(), 0);
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3 });

            it = v.insert(v.end(), 5);
            IS_TRUE(it == v.end() - 1);
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3,5 });

            it = v.insert(v.end() - 1, 4);
            IS_TRUE(it == v.end() - 2);
            AssertContainerEqual(v, STD_IVEC{ 0,1,2,3,4,5 });


            LAI_SVEC sv{ "stl" };

            auto sit = sv.insert(sv.begin(), "aaa");
            IS_TRUE(sit == sv.begin());
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "stl" });

            sit = sv.insert(sv.end(), "bbb");
            IS_TRUE(sit == sv.end() - 1);
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "stl", "bbb" });


            sit = sv.insert(sv.begin() + 1, "");
            IS_TRUE(sit == sv.begin() + 1);
            AssertContainerEqual(sv, STD_SVEC{ "aaa", "", "stl", "bbb" });

            sv.clear();
            sit = sv.insert(sv.insert(sv.insert(sv.begin(), "i"), "a"), "l");
            IS_TRUE(sit == sv.begin());
            AssertContainerEqual(sv, STD_SVEC{ "l", "a","i" });

            LAI_SVEC uv;
            for (int i = 1; i != 100; ++i)
                uv.insert(uv.begin(), std::string(i, 'a'));
            STD_SVEC uuv;
            for (int i = 1; i != 100; ++i)
                uuv.insert(uuv.begin(), std::string(i, 'a'));
            AssertContainerEqual(uv, uuv);

            LAI_SVEC v1{ "hello" };
            for (int i = 0; i != 100; ++i)
                v1.insert(v1.begin(), v1[0]);
            AssertContainerEqual(v1, STD_SVEC{ 101, "hello" });
        }

        TEST_METHOD(TestInsertCount)
        {
            LAI_SVEC u1;

            auto sit = u1.insert(u1.begin(), 0, "a");
            IS_TRUE(u1.empty());
            IS_TRUE(sit == u1.begin());
            IS_TRUE(sit == u1.end());
            AssertContainerEqual(u1, STD_SVEC());

            sit = u1.insert(u1.end(), 0, "a");
            IS_TRUE(u1.empty());
            IS_TRUE(sit == u1.begin());
            IS_TRUE(sit == u1.end());
            AssertContainerEqual(u1, STD_SVEC());

            sit = u1.insert(u1.end(), 100, "a");
            IS_TRUE(sit == u1.begin());
            AssertContainerEqual(u1, STD_SVEC{ 100, "a" });


            sit = u1.insert(u1.begin(), 100, "a");
            IS_TRUE(sit == u1.begin());
            AssertContainerEqual(u1, STD_SVEC{ 200, "a" });


            LAI_SVEC ss{ "a" };
            ss.insert(ss.begin(), 100, ss[0]);
            AssertContainerEqual(ss, STD_SVEC{ 101, "a" });
        }


        TEST_METHOD(TestInsertRange)
        {
            LAI_SVEC v;

            auto it = v.insert(v.begin(), { "l", "a", "i" });
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i" });

            it = v.insert(v.begin() + 1, {});
            IS_TRUE(it == v.begin() + 1);
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i" });

            it = v.insert(v.end(), { "s", "t", "l" });
            IS_TRUE(it == v.begin() + 3);
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i", "s", "t", "l" });

            it = v.insert(v.begin() + 1, { "laistl" });
            IS_TRUE(it == v.begin() + 1);
            AssertContainerEqual(v, STD_SVEC{ "l", "laistl", "a", "i", "s", "t", "l" });

            v.clear();
            it = v.insert(v.begin(), v.begin(), v.end());
            IS_TRUE(it == v.begin());
            IS_TRUE(it == v.end());
            IS_TRUE(v.empty());

            for (int i = 0; i != 100; ++i)
            {
                v.insert(v.begin(), { "a" });
            }
            AssertContainerEqual(v, STD_SVEC{ 100, "a" });

            v = { "lai", "stl" };
            it = v.insert(v.begin(), v.begin(), v.end());
            IS_TRUE(it == v.begin());
            AssertContainerEqual(v, STD_SVEC{ "lai", "stl", "lai", "stl" });

            it = v.insert(v.end(), v.begin(), v.end());
            IS_TRUE(it == v.end() - 4);
            AssertContainerEqual(v, STD_SVEC{ "lai", "stl", "lai", "stl", "lai", "stl", "lai", "stl" });

            it = v.erase(it, v.end());
            IS_TRUE(it == v.end());
            AssertContainerEqual(v, STD_SVEC{ "lai", "stl", "lai", "stl" });

            it = v.insert(v.begin() + 2, v.begin(), v.end());
            IS_TRUE(it == v.begin() + 2);
            AssertContainerEqual(v, STD_SVEC{ "lai", "stl", "lai", "stl", "lai", "stl", "lai", "stl" });

            v.clear();
            v = { "a" };
            for (int i = 0; i != 7; ++i)
            {
                v.insert(v.begin(), v.begin(), v.end());
            }
            AssertContainerEqual(v, STD_SVEC{ 128, "a" });
        }

        TEST_METHOD(TestSwap)
        {
            LAI_SVEC v{ "l", "a", "i" };
            LAI_SVEC v1{ "s", "t", "l" };

            v.swap(v1);
            AssertContainerEqual(v, STD_SVEC{ "s", "t", "l" });
            AssertContainerEqual(v1, STD_SVEC{ "l", "a", "i" });

            v1.swap(v);
            AssertContainerEqual(v, STD_SVEC{ "l", "a", "i" });
            AssertContainerEqual(v1, STD_SVEC{ "s", "t", "l" });

            v.clear();
            v.swap(v1);
            AssertContainerEqual(v, STD_SVEC{ "s", "t", "l" });
            AssertContainerEqual(v1, STD_SVEC{});

            v.clear();
            v1.swap(v);
            AssertContainerEqual(v, STD_SVEC{});
            AssertContainerEqual(v1, STD_SVEC{});

            v = { 100, "hello" };
            v.swap(v1);
            AssertContainerEqual(v, STD_SVEC{});
            AssertContainerEqual(v1, STD_SVEC{100, "hello"});
        }

        TEST_METHOD(TestAssign)
        {
            LAI_SVEC v;
            v.assign(0, "a");
            IS_TRUE(v.empty());

            v.assign(1, "a");
            AssertContainerEqual(v, STD_SVEC{ "a" });

            v.assign(10, "lai");
            AssertContainerEqual(v, STD_SVEC{ 10, "lai" });

            v.assign(101, "hello");
            AssertContainerEqual(v, STD_SVEC{ 101, "hello" });

            v.clear();
            v.assign(v.begin(), v.end());
            IS_TRUE(v.empty());

            v = { "lai", "stl" };
            v.assign(v.begin(), v.end());
            AssertContainerEqual(v, STD_SVEC{ "lai", "stl" });

            STD_SVEC u{ 100, "a" };
            v.assign(u.begin(), u.end() - 1);
            AssertContainerEqual(v, STD_SVEC{ 99, "a" });

            v.assign({});
            IS_TRUE(v.empty());

            v.assign({ "lai" });
            AssertContainerEqual(v, STD_SVEC{ "lai" });

            v.assign({ "a", "b", "c" });
            AssertContainerEqual(v, STD_SVEC{ "a", "b", "c" });
        }

        TEST_METHOD(TestRelationalOperators)
        {
            LAI_IVEC v, u;
            IS_TRUE(v == u);
            IS_TRUE(v <= u);
            IS_TRUE(v >= u);
            IS_FALSE(v < u);
            IS_FALSE(v > u);

            u = { 1 };
            IS_TRUE(v <= u);
            IS_TRUE(v < u);
            IS_FALSE(v == u);
            IS_FALSE(v > u);
            IS_FALSE(v >= u);

            v = { 1 };
            IS_TRUE(v >= u);
            IS_TRUE(v <= u);
            IS_TRUE(v == u);
            IS_FALSE(v > u);
            IS_FALSE(v > u);

            v = { 1, 1 };
            IS_TRUE(v >= u);
            IS_TRUE(v > u);
            IS_FALSE(v == u);
            IS_FALSE(v <= u);
            IS_FALSE(v < u);

            u = { 1, 2 };
            IS_TRUE(v < u);
            IS_TRUE(v <= u);
            IS_FALSE(v == u);
            IS_FALSE(v > u);
            IS_FALSE(v >= u);
        }

    };
}


#undef LAI_IVEC
#undef LAI_SVEC
#undef STD_IVEC
#undef STD_SVEC