// most test cases come from llvm test suite
// http://llvm.org/svn/llvm-project/libcxx/trunk/test/std/containers/associative/map/
#include "stdafx.h"
#include "../LaiSTL/Map.h"
#include <map>
#include <type_traits>
#include <functional>

#define MAP lai::map
#define S_MAP std::map


namespace UnitTest
{
    namespace
    {
        template<typename Compare>
        struct TestCompare : public Compare
        {
            int id;

            explicit TestCompare(int i = 0) :
                Compare(), id(i) { }

            bool operator==(const TestCompare & rhs) const noexcept
            {
                return id == rhs.id;
            }

            bool operator!=(const TestCompare & rhs) const noexcept
            {
                return !(*this == rhs);
            }
        };

        template<typename TKey>
        using TestGreater = TestCompare<std::greater<TKey>>;

        template<typename TKey>
        using TestLess = TestCompare<std::less<TKey>>;

        class DefaultOnly
        {
            int data_;

            DefaultOnly(const DefaultOnly&) = delete;
            DefaultOnly& operator=(const DefaultOnly&) = delete;
        public:
            static int count;

            DefaultOnly() : data_(-1) { ++count; }
            ~DefaultOnly() { data_ = 0; --count; }

            friend bool operator==(const DefaultOnly& x, const DefaultOnly& y)
            {
                return x.data_ == y.data_;
            }
            friend bool operator< (const DefaultOnly& x, const DefaultOnly& y)
            {
                return x.data_ < y.data_;
            }
        };

        int DefaultOnly::count = 0;


        class MoveOnly
        {
            MoveOnly(const MoveOnly&) = delete;
            MoveOnly& operator=(const MoveOnly&) = delete;

            int int_;
            double double_;
        public:
            MoveOnly() : int_(0), double_(0) {}
            MoveOnly(int i, double d) : int_(i), double_(d) {}
            MoveOnly(MoveOnly&& x)
                : int_(x.int_), double_(x.double_)
            {
                x.int_ = -1; x.double_ = -1;
            }
            MoveOnly& operator=(MoveOnly&& x)
            {
                int_ = x.int_; x.int_ = -1;
                double_ = x.double_; x.double_ = -1;
                return *this;
            }

            bool operator==(const MoveOnly& x) const
            {
                return int_ == x.int_ && double_ == x.double_;
            }
            bool operator<(const MoveOnly& x) const
            {
                return int_ < x.int_ || (int_ == x.int_ && double_ < x.double_);
            }

            int get() const { return int_; }
            bool moved() const { return int_ == -1; }
        };
    }

    TEST_CLASS(TestMap)
    {
        // default recursive constructor
        // http://llvm.org/svn/llvm-project/libcxx/trunk/test/std/containers/associative/map/map.cons/default_recursive.pass.cpp
        struct X
        {
            MAP<int, X> m;
            MAP<int, X>::iterator i;
            MAP<int, X>::const_iterator ci;
            MAP<int, X>::reverse_iterator ri;
            MAP<int, X>::const_reverse_iterator rci;
        };

        TEST_METHOD(TestRecursiveConstruction)
        {
            X x;
        }

        TEST_METHOD(TestDefaultConstructor)
        {
            MAP<int, int> m;
            IS_TRUE(m.empty());
            IS_TRUE(m.size() == 0);
            IS_TRUE(m.begin() == m.end());
            IS_TRUE(m.cbegin() == m.cend());
            IS_TRUE(m.rbegin() == m.rend());
            IS_TRUE(m.crbegin() == m.crend());

            using Compare = TestGreater<int>;
            MAP<int, int, Compare> m1;
            IS_TRUE(m1.empty());
            IS_TRUE(m1.size() == 0);
            IS_TRUE(m1.begin() == m1.end());
            IS_TRUE(m1.cbegin() == m1.cend());
            IS_TRUE(m1.rbegin() == m1.rend());
            IS_TRUE(m1.crbegin() == m1.crend());
            IS_TRUE(m1.key_comp() == Compare());
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            using PairId = std::pair<const int, double>;
            std::initializer_list<PairId> data = 
            {
                { 1,1 }, { 2,1 }, { 3,1 },
                { 1,1.5 }, { 2,1.5 }, { 3,1.5 },
                { 1,2 }, { 2,2 }, { 3,2 }
            };

            MAP<int, double> m = data;
            AssertContainerEqual(m, S_MAP<int, double>(data));

            using Compare = TestGreater<int>;
            MAP<int, double, Compare> m1(data, Compare(1));
            IS_TRUE(m1.key_comp() == Compare(1));
            AssertContainerEqual(m1, S_MAP<int, double, std::greater<int>>(data, Compare()));
        }

        TEST_METHOD(TestRangeConstructor)
        {
            using PairId = std::pair<const int, double>;
            std::initializer_list<PairId> data =
            {
                { 1,1 }, { 2,1 }, { 3,1 },
                { 1,1.5 }, { 2,1.5 }, { 3,1.5 },
                { 1,2 }, { 2,2 }, { 3,2 }
            };

            MAP<int, double> m(data.begin(), data.end());
            AssertContainerEqual(m,
                S_MAP<int, double>(data.begin(), data.end()));

            using Compare = TestGreater<int>;
            MAP<int, double, Compare> m1(data.begin(), data.end(), Compare(1));
            IS_TRUE(m1.key_comp() == Compare(1));
            AssertContainerEqual(m1,
                S_MAP<int, double, std::greater<int>>(data.begin(), data.end(), Compare(1)));
        }

        TEST_METHOD(TestCopyConstructor)
        {
            using PairId = std::pair<const int, double>;
            using Compare = TestGreater<int>;
            std::initializer_list<PairId> data =
            {
                { 1,1 },{ 2,1 },{ 3,1 },
                { 1,1.5 },{ 2,1.5 },{ 3,1.5 },
                { 1,2 },{ 2,2 },{ 3,2 }
            };

            {
                MAP<int, double> m;
                MAP<int, double> m1 = m;
                IS_TRUE(m1.empty());
                IS_TRUE(m.empty());

                m = data;
                MAP<int, double> m2 = m;
                AssertContainerEqual(m, m2);
                AssertContainerEqual(m, S_MAP<int, double>(data));
                AssertContainerEqual(m2, S_MAP<int, double>(data));
            }

            {
                MAP<int, double, Compare> m(Compare(1));
                MAP<int, double, Compare> m4 = m;
                IS_TRUE(m4.key_comp() == Compare(1));
                IS_TRUE(m.empty());
                IS_TRUE(m4.empty());

                m = data;
                MAP<int, double, Compare> m5 = m;
                IS_TRUE(m5.key_comp() == Compare(1));
                AssertContainerEqual(m, m5);
                AssertContainerEqual(m, S_MAP<int, double, Compare>(data, Compare(1)));
                AssertContainerEqual(m5, S_MAP<int, double, Compare>(data, Compare(1)));
            }
        }

        TEST_METHOD(TestMoveConstructor)
        {
            IS_TRUE(std::is_nothrow_move_constructible<
                MAP<Uncopyable, Uncopyable>>::value);

            IS_TRUE(std::is_nothrow_move_constructible<
                MAP<Uncopyable, Uncopyable, std::greater<Uncopyable>>>::value);

            IS_TRUE(std::is_nothrow_move_constructible <
                MAP<int, double>>::value);

            IS_TRUE(std::is_nothrow_move_constructible <
                MAP<int, double, std::greater<int>>>::value);

            using PairId = std::pair<const int, double>;
            using Compare = TestGreater<int>;
            std::initializer_list<PairId> data =
            {
                { 1,1 },{ 2,1 },{ 3,1 },
                { 1,1.5 },{ 2,1.5 },{ 3,1.5 },
                { 1,2 },{ 2,2 },{ 3,2 }
            };

            {
                MAP<int, double> m;
                MAP<int, double> m1 = std::move(m);
                IS_TRUE(m1.empty());
                m.~map();
                IS_TRUE(m1.empty());
            }

            {
                MAP<int, double> m = data;
                MAP<int, double> m2 = std::move(m);
                AssertContainerEqual(m2, S_MAP<int, double>(data));
                m.~map();
                AssertContainerEqual(m2, S_MAP<int, double>(data));
            }

            {
                MAP<int, double, Compare> m(Compare(1));
                MAP<int, double, Compare> m3 = std::move(m);
                IS_TRUE(m3.key_comp() == Compare(1));
                IS_TRUE(m3.empty());
                m.~map();
                IS_TRUE(m3.empty());

            }

            {
                MAP<int, double, Compare> m(data, Compare(1));
                MAP<int, double, Compare> m4 = std::move(m);
                IS_TRUE(m4.key_comp() == Compare(1));
                AssertContainerEqual(m4, S_MAP<int, double, Compare>(data, Compare(1)));
                m.~map();
                AssertContainerEqual(m4, S_MAP<int, double, Compare>(data, Compare(1)));
            }
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            using PairId = std::pair<const int, double>;
            using Compare = TestGreater<int>;
            std::initializer_list<PairId> data1 =
            {
                { 1,1 },{ 2,1 },{ 3,1 }
            };
            std::initializer_list<PairId> data2 =
            {
                { 1,2 },{ 2,2 },{ 3,2 }
            };

            {
                MAP<int, double> m(data1);
                MAP<int, double> m1, m2;
                m2 = (m1 = m);
                AssertContainerEqual(m, m1);
                AssertContainerEqual(m, m2);
                AssertContainerEqual(m, S_MAP<int, double>(data1));
            }

            {
                MAP<int, double, Compare> m(data1, Compare(2));
                MAP<int, double, Compare> m1(Compare(3)), m2(Compare(3));
                m2 = (m1 = m);
                IS_TRUE(m.key_comp() == Compare(2));
                IS_TRUE(m1.key_comp() == Compare(2));
                IS_TRUE(m2.key_comp() == Compare(2));
                AssertContainerEqual(m, m1);
                AssertContainerEqual(m, m2);
                AssertContainerEqual(m, S_MAP<int, double, Compare>(data1, Compare(2)));
            }

            {
                MAP<int, double> m;
                MAP<int, double> m1(data1), m2(data1);
                m2 = (m1 = m);
                IS_TRUE(m.empty());
                IS_TRUE(m1.empty());
                IS_TRUE(m2.empty());
            }

            {
                MAP<int, double, Compare> m(Compare(2));
                MAP<int, double, Compare> m1(data1, Compare(3)), m2(data1, Compare(3));
                m2 = (m1 = m);
                IS_TRUE(m.key_comp() == Compare(2));
                IS_TRUE(m1.key_comp() == Compare(2));
                IS_TRUE(m2.key_comp() == Compare(2));
                IS_TRUE(m.empty());
                IS_TRUE(m1.empty());
                IS_TRUE(m2.empty());
            }

            {
                MAP<int, double> m(data1);
                MAP<int, double> m1(data2), m2(data2);
                m2 = (m1 = m);
                AssertContainerEqual(m, m1);
                AssertContainerEqual(m, m2);
                AssertContainerEqual(m, S_MAP<int, double>(data1));
            }

            {
                MAP<int, double, Compare> m(data1, Compare(2));
                MAP<int, double, Compare> m1(data2, Compare(3)), m2(data2, Compare(3));
                m2 = (m1 = m);
                IS_TRUE(m.key_comp() == Compare(2));
                IS_TRUE(m1.key_comp() == Compare(2));
                IS_TRUE(m2.key_comp() == Compare(2));
                AssertContainerEqual(m, m1);
                AssertContainerEqual(m, m2);
                AssertContainerEqual(m, S_MAP<int, double, Compare>(data1, Compare(2)));
            }
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            using Compare = TestGreater<int>;
            using PairId = std::pair<const int, double>;
            std::initializer_list<PairId> data1 =
            {
                { 1,1 },{ 2,1 },{ 3,1 }
            };
            std::initializer_list<PairId> data2 =
            {
                { 1,2 },{ 2,2 },{ 3,2 }
            };

            IS_TRUE(std::is_nothrow_move_assignable<
                MAP<Uncopyable, Uncopyable>>::value);

            IS_TRUE(std::is_nothrow_move_assignable<
                MAP<Uncopyable, Uncopyable, Compare>>::value);

            IS_TRUE(std::is_nothrow_move_assignable<
                MAP<int, double>> ::value);

            IS_TRUE(std::is_nothrow_move_assignable<
                MAP<int, double, Compare>> ::value);

            {
                MAP<int, double> m(data1);
                MAP<int, double> m1, m2, m3;
                m1 = std::move(m);
                m.~map();
                AssertContainerEqual(m1, S_MAP<int, double>(data1));

                m3 = std::move(m2 = std::move(m1));
                AssertContainerEqual(m3, S_MAP<int, double>(data1));
            }

            {
                MAP<int, double, Compare> m(data1, Compare(2));
                MAP<int, double, Compare> m1(Compare(3)), m2(Compare(3)), m3(Compare(3));
                m1 = std::move(m);
                m.~map();
                IS_TRUE(m1.key_comp() == Compare(2));
                AssertContainerEqual(m1, S_MAP<int, double, Compare>(data1, Compare(2)));

                m3 = std::move(m2 = std::move(m1));
                IS_TRUE(m3.key_comp() == Compare(2));
                AssertContainerEqual(m3, S_MAP<int, double, Compare>(data1));
            }

            {
                MAP<int, double> m;
                MAP<int, double> m1(data1), m2(data1), m3(data1);
                m1 = std::move(m);
                m.~map();
                IS_TRUE(m1.empty());

                m3 = std::move(m2 = std::move(m1));
                IS_TRUE(m3.empty());
            }

            {
                MAP<int, double, Compare> m(Compare(2));
                MAP<int, double, Compare> m1(data1, Compare(3)), m2(data1, Compare(3)), m3(data1, Compare(3));
                m1 = std::move(m);
                m.~map();
                IS_TRUE(m1.key_comp() == Compare(2));
                IS_TRUE(m1.empty());

                m3 = std::move(m2 = std::move(m1));
                IS_TRUE(m3.key_comp() == Compare(2));
                IS_TRUE(m3.empty());
            }

            {
                MAP<int, double> m(data1);
                MAP<int, double> m1(data2), m2(data2), m3(data2);
                m1 = std::move(m);
                m.~map();
                AssertContainerEqual(m1, S_MAP<int, double>(data1));

                m3 = std::move(m2 = std::move(m1));
                AssertContainerEqual(m3, S_MAP<int, double>(data1));
            }

            {
                MAP<int, double, Compare> m(data1, Compare(2));
                MAP<int, double, Compare> m1(data2, Compare(3)), m2(data2, Compare(3)), m3(data2, Compare(3));
                m1 = std::move(m);
                m.~map();
                IS_TRUE(m1.key_comp() == Compare(2));
                AssertContainerEqual(m1, S_MAP<int, double, Compare>(data1, Compare(2)));

                m3 = std::move(m2 = std::move(m1));
                IS_TRUE(m3.key_comp() == Compare(2));
                AssertContainerEqual(m3, S_MAP<int, double, Compare>(data1, Compare(2)));
            }

            {
                MAP<int, Uncopyable> m;
                m.insert({ 1, Uncopyable(1) });
                m.insert({ 2, Uncopyable(2) });
                m.insert({ 3, Uncopyable(3) });

                MAP<int, Uncopyable> m1;
                m1 = std::move(m);
                m.~map();
                IS_TRUE(m1.begin()->second == Uncopyable(1));
                IS_TRUE((++m1.begin())->second == Uncopyable(2));
                IS_TRUE((++++m1.begin())->second == Uncopyable(3));
            }
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            using Compare = TestGreater<int>;
            using PairId = std::pair<const int, double>;
            std::initializer_list<PairId> data1 =
            {
                { 2,1 },{ 1,1 },{ 3,1 }
            };
            std::initializer_list<PairId> data2 =
            {
                { 1,2 },{ 3,2 },{ 2,2 }
            };

            {
                MAP<int, double> m;
                m = data1;
                AssertContainerEqual(m, S_MAP<int, double>(data1));

                MAP<int, double, Compare> m1(Compare(1));
                m1 = data1;
                IS_TRUE(m1.key_comp() == Compare(1));
                AssertContainerEqual(m1, S_MAP<int, double, Compare>(data1));
            }

            {
                MAP<int, double> m(data1);
                m = {};
                IS_TRUE(m.empty());

                MAP<int, double, Compare> m1(data1, Compare(1));
                m1 = {};
                IS_TRUE(m1.key_comp() == Compare(1));
                IS_TRUE(m1.empty());
            }

            {
                MAP<int, double> m(data1);
                m = data2;
                AssertContainerEqual(m, S_MAP<int, double>(data2));

                MAP<int, double, Compare> m1(data1, Compare(1));
                m1 = data2;
                IS_TRUE(m1.key_comp() == Compare(1));
                AssertContainerEqual(m1, S_MAP<int, double, Compare>(data2));
            }
        }

        TEST_METHOD(TestAt)
        {
            MAP<int, double> m{ {1,1}, {2,1}, {9,1} };
            IS_TRUE(m.at(1) == 1);
            IS_TRUE(m.at(2) == 1);
            IS_TRUE(m.at(9) == 1);
            EXPECT_EXPCETION<std::out_of_range>([&]() { m.at(0); });
            EXPECT_EXPCETION<std::out_of_range>([&]() { m.at(3); });
            EXPECT_EXPCETION<std::out_of_range>([&]() { m.at(10); });
            AssertContainerEqual(m, S_MAP<int, double>{ {1, 1}, { 2,1 }, { 9,1 }});

            const MAP<int, double> cm{ { 1,1 },{ 2,1 },{ 9,1 } };
            IS_TRUE(cm.at(1) == 1);
            IS_TRUE(cm.at(2) == 1);
            IS_TRUE(cm.at(9) == 1);
            EXPECT_EXPCETION<std::out_of_range>([&]() { cm.at(0); });
            EXPECT_EXPCETION<std::out_of_range>([&]() { cm.at(3); });
            EXPECT_EXPCETION<std::out_of_range>([&]() { cm.at(10); });
            AssertContainerEqual(cm, S_MAP<int, double>{ {1, 1}, { 2,1 }, { 9,1 }});
        }

        TEST_METHOD(TestTryEmplace)
        {
            { // pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
                typedef MAP<int, MoveOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r;
                for (int i = 0; i < 20; i += 2)
                    m.emplace(i, MoveOnly(i, (double)i));
                IS_TRUE(m.size() == 10);

                MoveOnly mv1(3, 3.0);
                for (int i = 0; i < 20; i += 2)
                {
                    r = m.try_emplace(i, std::move(mv1));
                    IS_TRUE(m.size() == 10);
                    IS_TRUE(!r.second);              // was not inserted
                    IS_TRUE(!mv1.moved());           // was not moved from
                    IS_TRUE(r.first->first == i);    // key
                }

                r = m.try_emplace(-1, std::move(mv1));
                IS_TRUE(m.size() == 11);
                IS_TRUE(r.second);                   // was inserted
                IS_TRUE(mv1.moved());                // was moved from
                IS_TRUE(r.first->first == -1);       // key
                IS_TRUE(r.first->second.get() == 3); // value

                MoveOnly mv2(5, 3.0);
                r = m.try_emplace(5, std::move(mv2));
                IS_TRUE(m.size() == 12);
                IS_TRUE(r.second);                   // was inserted
                IS_TRUE(mv2.moved());                // was moved from
                IS_TRUE(r.first->first == 5);        // key
                IS_TRUE(r.first->second.get() == 5); // value

                MoveOnly mv3(-1, 3.0);
                r = m.try_emplace(117, std::move(mv2));
                IS_TRUE(m.size() == 13);
                IS_TRUE(r.second);                    // was inserted
                IS_TRUE(mv2.moved());                 // was moved from
                IS_TRUE(r.first->first == 117);       // key
                IS_TRUE(r.first->second.get() == -1); // value
            }

            {  // pair<iterator, bool> try_emplace(key_type&& k, Args&&... args); 
                typedef MAP<MoveOnly, MoveOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r;
                for (int i = 0; i < 20; i += 2)
                    m.emplace(MoveOnly(i, (double)i), MoveOnly(i + 1, (double)i + 1));
                IS_TRUE(m.size() == 10);

                MoveOnly mvkey1(2, 2.0);
                MoveOnly mv1(4, 4.0);
                r = m.try_emplace(std::move(mvkey1), std::move(mv1));
                IS_TRUE(m.size() == 10);
                IS_TRUE(!r.second);                 // was not inserted
                IS_TRUE(!mv1.moved());              // was not moved from
                IS_TRUE(!mvkey1.moved());           // was not moved from
                IS_TRUE(r.first->first == mvkey1);  // key

                MoveOnly mvkey2(3, 3.0);
                r = m.try_emplace(std::move(mvkey2), std::move(mv1));
                IS_TRUE(m.size() == 11);
                IS_TRUE(r.second);                   // was inserted
                IS_TRUE(mv1.moved());                // was moved from
                IS_TRUE(mvkey2.moved());             // was moved from
                IS_TRUE(r.first->first.get() == 3); // key
                IS_TRUE(r.first->second.get() == 4); // value
            }
        }

        TEST_METHOD(TestSubscript)
        {
            {
                typedef std::pair<const int, double> V;
                V ar[] =
                {
                    V(1, 1.5),
                    V(2, 2.5),
                    V(3, 3.5),
                    V(4, 4.5),
                    V(5, 5.5),
                    V(7, 7.5),
                    V(8, 8.5),
                };
                MAP<int, double> m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                IS_TRUE(m.size() == 7);
                IS_TRUE(m[1] == 1.5);
                IS_TRUE(m.size() == 7);
                m[1] = -1.5;
                IS_TRUE(m[1] == -1.5);
                IS_TRUE(m.size() == 7);
                IS_TRUE(m[6] == 0);
                IS_TRUE(m.size() == 8);
                m[6] = 6.5;
                IS_TRUE(m[6] == 6.5);
                IS_TRUE(m.size() == 8);
            }

            {
                typedef std::pair<const int, double> V;
                V ar[] =
                {
                    V(1, 1.5),
                    V(2, 2.5),
                    V(3, 3.5),
                    V(4, 4.5),
                    V(5, 5.5),
                    V(7, 7.5),
                    V(8, 8.5),
                };
                MAP<int, double, std::less<>> m(ar, ar + sizeof(ar) / sizeof(ar[0]));

                IS_TRUE(m.size() == 7);
                IS_TRUE(m[1] == 1.5);
                IS_TRUE(m.size() == 7);
                m[1] = -1.5;
                IS_TRUE(m[1] == -1.5);
                IS_TRUE(m.size() == 7);
                IS_TRUE(m[6] == 0);
                IS_TRUE(m.size() == 8);
                m[6] = 6.5;
                IS_TRUE(m[6] == 6.5);
                IS_TRUE(m.size() == 8);
            }
        }

        TEST_METHOD(TestSubscriptRvKey)
        {
            {
                MAP<Uncopyable, double> m;
                IS_TRUE(m.size() == 0);
                IS_TRUE(m[1] == 0.0);
                IS_TRUE(m.size() == 1);
                m[1] = -1.5;
                IS_TRUE(m[1] == -1.5);
                IS_TRUE(m.size() == 1);
                IS_TRUE(m[6] == 0);
                IS_TRUE(m.size() == 2);
                m[6] = 6.5;
                IS_TRUE(m[6] == 6.5);
                IS_TRUE(m.size() == 2);
            }
            {
                typedef std::pair<const Uncopyable, double> V;
                MAP<Uncopyable, double, std::less<Uncopyable>> m;
                IS_TRUE(m.size() == 0);
                IS_TRUE(m[1] == 0.0);
                IS_TRUE(m.size() == 1);
                m[1] = -1.5;
                IS_TRUE(m[1] == -1.5);
                IS_TRUE(m.size() == 1);
                IS_TRUE(m[6] == 0);
                IS_TRUE(m.size() == 2);
                m[6] = 6.5;
                IS_TRUE(m[6] == 6.5);
                IS_TRUE(m.size() == 2);
            }
        }

        TEST_METHOD(TestEmpty)
        {
            {
                MAP<int, double> m;
                IS_TRUE(m.empty());
                m.insert({ 1,1 });
                IS_FALSE(m.empty());
                m.clear();
                IS_TRUE(m.empty());
            }
            {
                MAP<int, double, TestGreater<int>> m;
                IS_TRUE(m.empty());
                m.insert({ 1,1 });
                IS_FALSE(m.empty());
                m.clear();
                IS_TRUE(m.empty());
            }
        }

        TEST_METHOD(TestSize)
        {
            {
                MAP<int, double> m;
                IS_TRUE(m.size() == 0);
                m.insert({ 2, 1.5 });
                IS_TRUE(m.size() == 1);
                m.insert({ 1, 1.5 });
                IS_TRUE(m.size() == 2);
                m.insert({ 3, 1.5 });
                IS_TRUE(m.size() == 3);
                m.erase(m.begin());
                IS_TRUE(m.size() == 2);
                m.erase(m.begin());
                IS_TRUE(m.size() == 1);
                m.erase(m.begin());
                IS_TRUE(m.size() == 0);
            }
            {
                MAP<int, double, TestGreater<int>> m;
                IS_TRUE(m.size() == 0);
                m.insert({ 2, 1.5 });
                IS_TRUE(m.size() == 1);
                m.insert({ 1, 1.5 });
                IS_TRUE(m.size() == 2);
                m.insert({ 3, 1.5 });
                IS_TRUE(m.size() == 3);
                m.erase(m.begin());
                IS_TRUE(m.size() == 2);
                m.erase(m.begin());
                IS_TRUE(m.size() == 1);
                m.erase(m.begin());
                IS_TRUE(m.size() == 0);
            }
        }

        TEST_METHOD(TestClear)
        {
            {
                MAP<int, double> m
                {
                    {1,1.5}, {2,1.5}, {3,1.5}, {4,1.5}, {5,1.5}, {6,1.5}, {7,1.5}, {8,1.5}
                };
                IS_TRUE(m.size() == 8);
                m.clear();
                IS_TRUE(m.size() == 0);
            }

            {
                MAP<int, double, TestGreater<int>> m
                {
                    { 1,1.5 },{ 2,1.5 },{ 3,1.5 },{ 4,1.5 },{ 5,1.5 },{ 6,1.5 },{ 7,1.5 },{ 8,1.5 }
                };
                IS_TRUE(m.size() == 8);
                m.clear();
                IS_TRUE(m.size() == 0);
            }
        }

        TEST_METHOD(TestEmplace)
        {
            {
                typedef MAP<int, DefaultOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                IS_TRUE(DefaultOnly::count == 0);
                R r = m.emplace();
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 0);
                IS_TRUE(m.begin()->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 1);
                r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                    std::forward_as_tuple());
                IS_TRUE(r.second);
                IS_TRUE(r.first == next(m.begin()));
                IS_TRUE(m.size() == 2);
                IS_TRUE(next(m.begin())->first == 1);
                IS_TRUE(next(m.begin())->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 2);
                r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                    std::forward_as_tuple());
                IS_TRUE(!r.second);
                IS_TRUE(r.first == next(m.begin()));
                IS_TRUE(m.size() == 2);
                IS_TRUE(next(m.begin())->first == 1);
                IS_TRUE(next(m.begin())->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 2);
            }
            IS_TRUE(DefaultOnly::count == 0);
            {
                typedef MAP<int, MoveOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r = m.emplace(std::piecewise_construct, std::forward_as_tuple(2),
                    std::forward_as_tuple());
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == MoveOnly());
                r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                    std::forward_as_tuple(2, 3.5));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == MoveOnly(2, 3.5));
                r = m.emplace(std::piecewise_construct, std::forward_as_tuple(1),
                    std::forward_as_tuple(2, 3.5));
                IS_TRUE(!r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == MoveOnly(2, 3.5));
            }
            {
                typedef MAP<int, double> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r = m.emplace(M::value_type(2, 3.5));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 3.5);
            }
        }

        TEST_METHOD(TestEmplaceHint)
        {
            {
                typedef MAP<int, DefaultOnly> M;
                typedef M::iterator R;
                M m;
                IS_TRUE(DefaultOnly::count == 0);
                R r = m.emplace_hint(m.end());
                IS_TRUE(r == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 0);
                IS_TRUE(m.begin()->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 1);
                r = m.emplace_hint(m.end(), std::piecewise_construct,
                    std::forward_as_tuple(1),
                    std::forward_as_tuple());
                IS_TRUE(r == next(m.begin()));
                IS_TRUE(m.size() == 2);
                IS_TRUE(next(m.begin())->first == 1);
                IS_TRUE(next(m.begin())->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 2);
                r = m.emplace_hint(m.end(), std::piecewise_construct,
                    std::forward_as_tuple(1),
                    std::forward_as_tuple());
                IS_TRUE(r == next(m.begin()));
                IS_TRUE(m.size() == 2);
                IS_TRUE(next(m.begin())->first == 1);
                IS_TRUE(next(m.begin())->second == DefaultOnly());
                IS_TRUE(DefaultOnly::count == 2);
            }
            IS_TRUE(DefaultOnly::count == 0);
            {
                typedef MAP<int, MoveOnly> M;
                typedef M::iterator R;
                M m;
                R r = m.emplace_hint(m.end(), std::piecewise_construct,
                    std::forward_as_tuple(2),
                    std::forward_as_tuple());
                IS_TRUE(r == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == MoveOnly());
                r = m.emplace_hint(m.end(), std::piecewise_construct,
                    std::forward_as_tuple(1),
                    std::forward_as_tuple(2, 3.5));
                IS_TRUE(r == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == MoveOnly(2, 3.5));
                r = m.emplace_hint(m.end(), std::piecewise_construct,
                    std::forward_as_tuple(1),
                    std::forward_as_tuple(2, 3.5));
                IS_TRUE(r == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == MoveOnly(2, 3.5));
            }
            {
                typedef MAP<int, double> M;
                typedef M::iterator R;
                M m;
                R r = m.emplace_hint(m.end(), M::value_type(2, 3.5));
                IS_TRUE(r == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 3.5);
            }
        }

        TEST_METHOD(TestErase)
        {
            {
                typedef MAP<int, double> M;
                typedef std::pair<int, double> P;
                typedef M::iterator I;
                P ar[] =
                {
                    P(1, 1.5),
                    P(2, 2.5),
                    P(3, 3.5),
                    P(4, 4.5),
                    P(5, 5.5),
                    P(6, 6.5),
                    P(7, 7.5),
                    P(8, 8.5),
                };
                M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                IS_TRUE(m.size() == 8);
                I i = m.erase(next(m.cbegin(), 3));
                IS_TRUE(m.size() == 7);
                IS_TRUE(i == next(m.begin(), 3));
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == 1.5);
                IS_TRUE(next(m.begin())->first == 2);
                IS_TRUE(next(m.begin())->second == 2.5);
                IS_TRUE(next(m.begin(), 2)->first == 3);
                IS_TRUE(next(m.begin(), 2)->second == 3.5);
                IS_TRUE(next(m.begin(), 3)->first == 5);
                IS_TRUE(next(m.begin(), 3)->second == 5.5);
                IS_TRUE(next(m.begin(), 4)->first == 6);
                IS_TRUE(next(m.begin(), 4)->second == 6.5);
                IS_TRUE(next(m.begin(), 5)->first == 7);
                IS_TRUE(next(m.begin(), 5)->second == 7.5);
                IS_TRUE(next(m.begin(), 6)->first == 8);
                IS_TRUE(next(m.begin(), 6)->second == 8.5);

                i = m.erase(next(m.cbegin(), 0));
                IS_TRUE(m.size() == 6);
                IS_TRUE(i == m.begin());
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 3);
                IS_TRUE(next(m.begin())->second == 3.5);
                IS_TRUE(next(m.begin(), 2)->first == 5);
                IS_TRUE(next(m.begin(), 2)->second == 5.5);
                IS_TRUE(next(m.begin(), 3)->first == 6);
                IS_TRUE(next(m.begin(), 3)->second == 6.5);
                IS_TRUE(next(m.begin(), 4)->first == 7);
                IS_TRUE(next(m.begin(), 4)->second == 7.5);
                IS_TRUE(next(m.begin(), 5)->first == 8);
                IS_TRUE(next(m.begin(), 5)->second == 8.5);

                i = m.erase(next(m.cbegin(), 5));
                IS_TRUE(m.size() == 5);
                IS_TRUE(i == m.end());
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 3);
                IS_TRUE(next(m.begin())->second == 3.5);
                IS_TRUE(next(m.begin(), 2)->first == 5);
                IS_TRUE(next(m.begin(), 2)->second == 5.5);
                IS_TRUE(next(m.begin(), 3)->first == 6);
                IS_TRUE(next(m.begin(), 3)->second == 6.5);
                IS_TRUE(next(m.begin(), 4)->first == 7);
                IS_TRUE(next(m.begin(), 4)->second == 7.5);

                i = m.erase(next(m.cbegin(), 1));
                IS_TRUE(m.size() == 4);
                IS_TRUE(i == next(m.begin()));
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);
                IS_TRUE(next(m.begin(), 2)->first == 6);
                IS_TRUE(next(m.begin(), 2)->second == 6.5);
                IS_TRUE(next(m.begin(), 3)->first == 7);
                IS_TRUE(next(m.begin(), 3)->second == 7.5);

                i = m.erase(next(m.cbegin(), 2));
                IS_TRUE(m.size() == 3);
                IS_TRUE(i == next(m.begin(), 2));
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);
                IS_TRUE(next(m.begin(), 2)->first == 7);
                IS_TRUE(next(m.begin(), 2)->second == 7.5);

                i = m.erase(next(m.cbegin(), 2));
                IS_TRUE(m.size() == 2);
                IS_TRUE(i == next(m.begin(), 2));
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);

                i = m.erase(next(m.cbegin(), 0));
                IS_TRUE(m.size() == 1);
                IS_TRUE(i == next(m.begin(), 0));
                IS_TRUE(m.begin()->first == 5);
                IS_TRUE(m.begin()->second == 5.5);

                i = m.erase(m.cbegin());
                IS_TRUE(m.size() == 0);
                IS_TRUE(i == m.begin());
                IS_TRUE(i == m.end());
            }
        }

        TEST_METHOD(TestEraseRange)
        {
            {
                typedef MAP<int, double> M;
                typedef std::pair<int, double> P;
                typedef M::iterator I;
                P ar[] =
                {
                    P(1, 1.5),
                    P(2, 2.5),
                    P(3, 3.5),
                    P(4, 4.5),
                    P(5, 5.5),
                    P(6, 6.5),
                    P(7, 7.5),
                    P(8, 8.5),
                };
                M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                IS_TRUE(m.size() == 8);
                I i = m.erase(m.cbegin(), m.cbegin());
                IS_TRUE(m.size() == 8);
                IS_TRUE(i == m.begin());
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == 1.5);
                IS_TRUE(next(m.begin())->first == 2);
                IS_TRUE(next(m.begin())->second == 2.5);
                IS_TRUE(next(m.begin(), 2)->first == 3);
                IS_TRUE(next(m.begin(), 2)->second == 3.5);
                IS_TRUE(next(m.begin(), 3)->first == 4);
                IS_TRUE(next(m.begin(), 3)->second == 4.5);
                IS_TRUE(next(m.begin(), 4)->first == 5);
                IS_TRUE(next(m.begin(), 4)->second == 5.5);
                IS_TRUE(next(m.begin(), 5)->first == 6);
                IS_TRUE(next(m.begin(), 5)->second == 6.5);
                IS_TRUE(next(m.begin(), 6)->first == 7);
                IS_TRUE(next(m.begin(), 6)->second == 7.5);
                IS_TRUE(next(m.begin(), 7)->first == 8);
                IS_TRUE(next(m.begin(), 7)->second == 8.5);

                i = m.erase(m.cbegin(), next(m.cbegin(), 2));
                IS_TRUE(m.size() == 6);
                IS_TRUE(i == m.begin());
                IS_TRUE(next(m.begin(), 0)->first == 3);
                IS_TRUE(next(m.begin(), 0)->second == 3.5);
                IS_TRUE(next(m.begin(), 1)->first == 4);
                IS_TRUE(next(m.begin(), 1)->second == 4.5);
                IS_TRUE(next(m.begin(), 2)->first == 5);
                IS_TRUE(next(m.begin(), 2)->second == 5.5);
                IS_TRUE(next(m.begin(), 3)->first == 6);
                IS_TRUE(next(m.begin(), 3)->second == 6.5);
                IS_TRUE(next(m.begin(), 4)->first == 7);
                IS_TRUE(next(m.begin(), 4)->second == 7.5);
                IS_TRUE(next(m.begin(), 5)->first == 8);
                IS_TRUE(next(m.begin(), 5)->second == 8.5);

                i = m.erase(next(m.cbegin(), 2), next(m.cbegin(), 6));
                IS_TRUE(m.size() == 2);
                IS_TRUE(i == next(m.begin(), 2));
                IS_TRUE(next(m.begin(), 0)->first == 3);
                IS_TRUE(next(m.begin(), 0)->second == 3.5);
                IS_TRUE(next(m.begin(), 1)->first == 4);
                IS_TRUE(next(m.begin(), 1)->second == 4.5);

                i = m.erase(m.cbegin(), m.cend());
                IS_TRUE(m.size() == 0);
                IS_TRUE(i == m.begin());
                IS_TRUE(i == m.end());
            }
        }

        TEST_METHOD(TestEraseKey)
        {
            {
                typedef MAP<int, double> M;
                typedef std::pair<int, double> P;
                typedef M::size_type R;
                P ar[] =
                {
                    P(1, 1.5),
                    P(2, 2.5),
                    P(3, 3.5),
                    P(4, 4.5),
                    P(5, 5.5),
                    P(6, 6.5),
                    P(7, 7.5),
                    P(8, 8.5),
                };
                M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                IS_TRUE(m.size() == 8);
                R s = m.erase(9);
                IS_TRUE(s == 0);
                IS_TRUE(m.size() == 8);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == 1.5);
                IS_TRUE(next(m.begin())->first == 2);
                IS_TRUE(next(m.begin())->second == 2.5);
                IS_TRUE(next(m.begin(), 2)->first == 3);
                IS_TRUE(next(m.begin(), 2)->second == 3.5);
                IS_TRUE(next(m.begin(), 3)->first == 4);
                IS_TRUE(next(m.begin(), 3)->second == 4.5);
                IS_TRUE(next(m.begin(), 4)->first == 5);
                IS_TRUE(next(m.begin(), 4)->second == 5.5);
                IS_TRUE(next(m.begin(), 5)->first == 6);
                IS_TRUE(next(m.begin(), 5)->second == 6.5);
                IS_TRUE(next(m.begin(), 6)->first == 7);
                IS_TRUE(next(m.begin(), 6)->second == 7.5);
                IS_TRUE(next(m.begin(), 7)->first == 8);
                IS_TRUE(next(m.begin(), 7)->second == 8.5);

                s = m.erase(4);
                IS_TRUE(m.size() == 7);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 1);
                IS_TRUE(m.begin()->second == 1.5);
                IS_TRUE(next(m.begin())->first == 2);
                IS_TRUE(next(m.begin())->second == 2.5);
                IS_TRUE(next(m.begin(), 2)->first == 3);
                IS_TRUE(next(m.begin(), 2)->second == 3.5);
                IS_TRUE(next(m.begin(), 3)->first == 5);
                IS_TRUE(next(m.begin(), 3)->second == 5.5);
                IS_TRUE(next(m.begin(), 4)->first == 6);
                IS_TRUE(next(m.begin(), 4)->second == 6.5);
                IS_TRUE(next(m.begin(), 5)->first == 7);
                IS_TRUE(next(m.begin(), 5)->second == 7.5);
                IS_TRUE(next(m.begin(), 6)->first == 8);
                IS_TRUE(next(m.begin(), 6)->second == 8.5);

                s = m.erase(1);
                IS_TRUE(m.size() == 6);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 3);
                IS_TRUE(next(m.begin())->second == 3.5);
                IS_TRUE(next(m.begin(), 2)->first == 5);
                IS_TRUE(next(m.begin(), 2)->second == 5.5);
                IS_TRUE(next(m.begin(), 3)->first == 6);
                IS_TRUE(next(m.begin(), 3)->second == 6.5);
                IS_TRUE(next(m.begin(), 4)->first == 7);
                IS_TRUE(next(m.begin(), 4)->second == 7.5);
                IS_TRUE(next(m.begin(), 5)->first == 8);
                IS_TRUE(next(m.begin(), 5)->second == 8.5);

                s = m.erase(8);
                IS_TRUE(m.size() == 5);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 3);
                IS_TRUE(next(m.begin())->second == 3.5);
                IS_TRUE(next(m.begin(), 2)->first == 5);
                IS_TRUE(next(m.begin(), 2)->second == 5.5);
                IS_TRUE(next(m.begin(), 3)->first == 6);
                IS_TRUE(next(m.begin(), 3)->second == 6.5);
                IS_TRUE(next(m.begin(), 4)->first == 7);
                IS_TRUE(next(m.begin(), 4)->second == 7.5);

                s = m.erase(3);
                IS_TRUE(m.size() == 4);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);
                IS_TRUE(next(m.begin(), 2)->first == 6);
                IS_TRUE(next(m.begin(), 2)->second == 6.5);
                IS_TRUE(next(m.begin(), 3)->first == 7);
                IS_TRUE(next(m.begin(), 3)->second == 7.5);

                s = m.erase(6);
                IS_TRUE(m.size() == 3);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);
                IS_TRUE(next(m.begin(), 2)->first == 7);
                IS_TRUE(next(m.begin(), 2)->second == 7.5);

                s = m.erase(7);
                IS_TRUE(m.size() == 2);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 2);
                IS_TRUE(m.begin()->second == 2.5);
                IS_TRUE(next(m.begin())->first == 5);
                IS_TRUE(next(m.begin())->second == 5.5);

                s = m.erase(2);
                IS_TRUE(m.size() == 1);
                IS_TRUE(s == 1);
                IS_TRUE(m.begin()->first == 5);
                IS_TRUE(m.begin()->second == 5.5);

                s = m.erase(5);
                IS_TRUE(m.size() == 0);
                IS_TRUE(s == 1);
            }
        }

        TEST_METHOD(TestInsertCv)
        {
            {
                typedef MAP<int, double> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r = m.insert(M::value_type(2, 2.5));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(r.first->first == 2);
                IS_TRUE(r.first->second == 2.5);

                r = m.insert(M::value_type(1, 1.5));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(r.first->first == 1);
                IS_TRUE(r.first->second == 1.5);

                r = m.insert(M::value_type(3, 3.5));
                IS_TRUE(r.second);
                IS_TRUE(r.first == prev(m.end()));
                IS_TRUE(m.size() == 3);
                IS_TRUE(r.first->first == 3);
                IS_TRUE(r.first->second == 3.5);

                r = m.insert(M::value_type(3, 3.5));
                IS_TRUE(!r.second);
                IS_TRUE(r.first == prev(m.end()));
                IS_TRUE(m.size() == 3);
                IS_TRUE(r.first->first == 3);
                IS_TRUE(r.first->second == 3.5);
            }
        }

        TEST_METHOD(TestInertRv)
        {
            {
                typedef MAP<int, Uncopyable> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r = m.insert(M::value_type(2, 2));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 1);
                IS_TRUE(r.first->first == 2);
                IS_TRUE(r.first->second == 2);

                r = m.insert(M::value_type(1, 1));
                IS_TRUE(r.second);
                IS_TRUE(r.first == m.begin());
                IS_TRUE(m.size() == 2);
                IS_TRUE(r.first->first == 1);
                IS_TRUE(r.first->second == 1);

                r = m.insert(M::value_type(3, 3));
                IS_TRUE(r.second);
                IS_TRUE(r.first == prev(m.end()));
                IS_TRUE(m.size() == 3);
                IS_TRUE(r.first->first == 3);
                IS_TRUE(r.first->second == 3);

                r = m.insert(M::value_type(3, 3));
                IS_TRUE(!r.second);
                IS_TRUE(r.first == prev(m.end()));
                IS_TRUE(m.size() == 3);
                IS_TRUE(r.first->first == 3);
                IS_TRUE(r.first->second == 3);
            }
        }

        TEST_METHOD(TestInsertInitList)
        {
            typedef std::pair<const int, double> V;
            MAP<int, double> m =
            {
                { 1, 1 },
                { 1, 1.5 },
                { 1, 2 },
                { 3, 1 },
                { 3, 1.5 },
                { 3, 2 }
            };
            m.insert({
                { 2, 1 },
                { 2, 1.5 },
                { 2, 2 },
            });
            IS_TRUE(m.size() == 3);
            IS_TRUE(distance(m.begin(), m.end()) == 3);
            IS_TRUE(*m.begin() == V(1, 1));
            IS_TRUE(*next(m.begin()) == V(2, 1));
            IS_TRUE(*next(m.begin(), 2) == V(3, 1));
        }

        TEST_METHOD(TestInsertHintCv)
        {
            typedef MAP<int, double> M;
            typedef M::iterator R;
            M m;
            R r = m.insert(m.end(), M::value_type(2, 2.5));
            IS_TRUE(r == m.begin());
            IS_TRUE(m.size() == 1);
            IS_TRUE(r->first == 2);
            IS_TRUE(r->second == 2.5);

            r = m.insert(m.end(), M::value_type(1, 1.5));
            IS_TRUE(r == m.begin());
            IS_TRUE(m.size() == 2);
            IS_TRUE(r->first == 1);
            IS_TRUE(r->second == 1.5);

            r = m.insert(m.end(), M::value_type(3, 3.5));
            IS_TRUE(r == prev(m.end()));
            IS_TRUE(m.size() == 3);
            IS_TRUE(r->first == 3);
            IS_TRUE(r->second == 3.5);

            r = m.insert(m.end(), M::value_type(3, 3.5));
            IS_TRUE(r == prev(m.end()));
            IS_TRUE(m.size() == 3);
            IS_TRUE(r->first == 3);
            IS_TRUE(r->second == 3.5);
        }

        TEST_METHOD(TestInsertHintRv)
        {
            typedef MAP<int, Uncopyable> M;
            typedef std::pair<int, Uncopyable> P;
            typedef M::iterator R;
            M m;
            R r = m.insert(m.end(), P(2, 2));
            IS_TRUE(r == m.begin());
            IS_TRUE(m.size() == 1);
            IS_TRUE(r->first == 2);
            IS_TRUE(r->second == 2);

            r = m.insert(m.end(), P(1, 1));
            IS_TRUE(r == m.begin());
            IS_TRUE(m.size() == 2);
            IS_TRUE(r->first == 1);
            IS_TRUE(r->second == 1);

            r = m.insert(m.end(), P(3, 3));
            IS_TRUE(r == prev(m.end()));
            IS_TRUE(m.size() == 3);
            IS_TRUE(r->first == 3);
            IS_TRUE(r->second == 3);

            r = m.insert(m.end(), P(3, 3));
            IS_TRUE(r == prev(m.end()));
            IS_TRUE(m.size() == 3);
            IS_TRUE(r->first == 3);
            IS_TRUE(r->second == 3);

        }

        TEST_METHOD(TestInsertRange)
        {
            typedef MAP<int, double> M;
            typedef std::pair<int, double> P;
            std::initializer_list<P> ar =
            {
                P(1, 1),
                P(1, 1.5),
                P(1, 2),
                P(2, 1),
                P(2, 1.5),
                P(2, 2),
                P(3, 1),
                P(3, 1.5),
                P(3, 2),
            };
            M m;
            m.insert(ar.begin(), ar.end());
            IS_TRUE(m.size() == 3);
            IS_TRUE(m.begin()->first == 1);
            IS_TRUE(m.begin()->second == 1);
            IS_TRUE(next(m.begin())->first == 2);
            IS_TRUE(next(m.begin())->second == 1);
            IS_TRUE(next(m.begin(), 2)->first == 3);
            IS_TRUE(next(m.begin(), 2)->second == 1);
        }

        TEST_METHOD(TestInsertOrAssign)
        {
            {
                typedef MAP<int, MoveOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r;
                for (int i = 0; i < 20; i += 2)
                    m.emplace(i, MoveOnly(i, (double)i));
                IS_TRUE(m.size() == 10);

                for (int i = 0; i < 20; i += 2)
                {
                    MoveOnly mv(i + 1, i + 1);
                    r = m.insert_or_assign(i, std::move(mv));
                    IS_TRUE(m.size() == 10);
                    IS_TRUE(!r.second);                    // was not inserted
                    IS_TRUE(mv.moved());                   // was moved from
                    IS_TRUE(r.first->first == i);          // key
                    IS_TRUE(r.first->second.get() == i + 1); // value
                }

                MoveOnly mv1(5, 5.0);
                r = m.insert_or_assign(-1, std::move(mv1));
                IS_TRUE(m.size() == 11);
                IS_TRUE(r.second);                    // was inserted
                IS_TRUE(mv1.moved());                 // was moved from
                IS_TRUE(r.first->first == -1); // key
                IS_TRUE(r.first->second.get() == 5);  // value

                MoveOnly mv2(9, 9.0);
                r = m.insert_or_assign(3, std::move(mv2));
                IS_TRUE(m.size() == 12);
                IS_TRUE(r.second);                   // was inserted
                IS_TRUE(mv2.moved());                // was moved from
                IS_TRUE(r.first->first == 3); // key
                IS_TRUE(r.first->second.get() == 9); // value

                MoveOnly mv3(-1, 5.0);
                r = m.insert_or_assign(117, std::move(mv3));
                IS_TRUE(m.size() == 13);
                IS_TRUE(r.second);                     // was inserted
                IS_TRUE(mv3.moved());                  // was moved from
                IS_TRUE(r.first->first == 117); // key
                IS_TRUE(r.first->second.get() == -1);  // value
            }

            {
                typedef MAP<MoveOnly, MoveOnly> M;
                typedef std::pair<M::iterator, bool> R;
                M m;
                R r;
                for (int i = 0; i < 20; i += 2)
                    m.emplace(MoveOnly(i, (double)i), MoveOnly(i + 1, (double)i + 1));
                IS_TRUE(m.size() == 10);

                MoveOnly mvkey1(2, 2.0);
                MoveOnly mv1(4, 4.0);
                r = m.insert_or_assign(std::move(mvkey1), std::move(mv1));
                IS_TRUE(m.size() == 10);
                IS_TRUE(!r.second);                  // was not inserted
                IS_TRUE(!mvkey1.moved());            // was not moved from
                IS_TRUE(mv1.moved());                // was moved from
                IS_TRUE(r.first->first == mvkey1);   // key
                IS_TRUE(r.first->second.get() == 4); // value

                MoveOnly mvkey2(3, 3.0);
                MoveOnly mv2(5, 5.0);
                r = m.try_emplace(std::move(mvkey2), std::move(mv2));
                IS_TRUE(m.size() == 11);
                IS_TRUE(r.second);                   // was inserted
                IS_TRUE(mv2.moved());                // was moved from
                IS_TRUE(mvkey2.moved());             // was moved from
                IS_TRUE(r.first->first.get() == 3); // key
                IS_TRUE(r.first->second.get() == 5); // value
            }
        }

        TEST_METHOD(TestCount)
        {
            {
                MAP<int, double> m
                {
                    {1,1}, {2,2}, {3,3},
                    {4,4}, {5,5}, {6,6}
                };

                for (int i = 1; i <= 6; ++i)
                    IS_TRUE(m.count(i) == 1);

                IS_TRUE(m.count(0) == 0);
                IS_TRUE(m.count(7) == 0);
            }

            {
                MAP<int, double> m;
                IS_TRUE(m.count(0) == 0);
                IS_TRUE(m.count(1) == 0);
            }
        }

        TEST_METHOD(TestEqualRange)
        {
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                typedef std::pair<M::iterator, M::iterator> R;
                V ar[] =
                {
                    V(5, 5),
                    V(7, 6),
                    V(9, 7),
                    V(11, 8),
                    V(13, 9),
                    V(15, 10),
                    V(17, 11),
                    V(19, 12)
                };
                M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                R r = m.equal_range(5);
                IS_TRUE(r.first == next(m.begin(), 0));
                IS_TRUE(r.second == next(m.begin(), 1));
                r = m.equal_range(7);
                IS_TRUE(r.first == next(m.begin(), 1));
                IS_TRUE(r.second == next(m.begin(), 2));
                r = m.equal_range(9);
                IS_TRUE(r.first == next(m.begin(), 2));
                IS_TRUE(r.second == next(m.begin(), 3));
                r = m.equal_range(11);
                IS_TRUE(r.first == next(m.begin(), 3));
                IS_TRUE(r.second == next(m.begin(), 4));
                r = m.equal_range(13);
                IS_TRUE(r.first == next(m.begin(), 4));
                IS_TRUE(r.second == next(m.begin(), 5));
                r = m.equal_range(15);
                IS_TRUE(r.first == next(m.begin(), 5));
                IS_TRUE(r.second == next(m.begin(), 6));
                r = m.equal_range(17);
                IS_TRUE(r.first == next(m.begin(), 6));
                IS_TRUE(r.second == next(m.begin(), 7));
                r = m.equal_range(19);
                IS_TRUE(r.first == next(m.begin(), 7));
                IS_TRUE(r.second == next(m.begin(), 8));
                r = m.equal_range(4);
                IS_TRUE(r.first == next(m.begin(), 0));
                IS_TRUE(r.second == next(m.begin(), 0));
                r = m.equal_range(6);
                IS_TRUE(r.first == next(m.begin(), 1));
                IS_TRUE(r.second == next(m.begin(), 1));
                r = m.equal_range(8);
                IS_TRUE(r.first == next(m.begin(), 2));
                IS_TRUE(r.second == next(m.begin(), 2));
                r = m.equal_range(10);
                IS_TRUE(r.first == next(m.begin(), 3));
                IS_TRUE(r.second == next(m.begin(), 3));
                r = m.equal_range(12);
                IS_TRUE(r.first == next(m.begin(), 4));
                IS_TRUE(r.second == next(m.begin(), 4));
                r = m.equal_range(14);
                IS_TRUE(r.first == next(m.begin(), 5));
                IS_TRUE(r.second == next(m.begin(), 5));
                r = m.equal_range(16);
                IS_TRUE(r.first == next(m.begin(), 6));
                IS_TRUE(r.second == next(m.begin(), 6));
                r = m.equal_range(18);
                IS_TRUE(r.first == next(m.begin(), 7));
                IS_TRUE(r.second == next(m.begin(), 7));
                r = m.equal_range(20);
                IS_TRUE(r.first == next(m.begin(), 8));
                IS_TRUE(r.second == next(m.begin(), 8));
            }
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                typedef std::pair<M::const_iterator, M::const_iterator> R;
                V ar[] =
                {
                    V(5, 5),
                    V(7, 6),
                    V(9, 7),
                    V(11, 8),
                    V(13, 9),
                    V(15, 10),
                    V(17, 11),
                    V(19, 12)
                };
                const M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                R r = m.equal_range(5);
                IS_TRUE(r.first == next(m.begin(), 0));
                IS_TRUE(r.second == next(m.begin(), 1));
                r = m.equal_range(7);
                IS_TRUE(r.first == next(m.begin(), 1));
                IS_TRUE(r.second == next(m.begin(), 2));
                r = m.equal_range(9);
                IS_TRUE(r.first == next(m.begin(), 2));
                IS_TRUE(r.second == next(m.begin(), 3));
                r = m.equal_range(11);
                IS_TRUE(r.first == next(m.begin(), 3));
                IS_TRUE(r.second == next(m.begin(), 4));
                r = m.equal_range(13);
                IS_TRUE(r.first == next(m.begin(), 4));
                IS_TRUE(r.second == next(m.begin(), 5));
                r = m.equal_range(15);
                IS_TRUE(r.first == next(m.begin(), 5));
                IS_TRUE(r.second == next(m.begin(), 6));
                r = m.equal_range(17);
                IS_TRUE(r.first == next(m.begin(), 6));
                IS_TRUE(r.second == next(m.begin(), 7));
                r = m.equal_range(19);
                IS_TRUE(r.first == next(m.begin(), 7));
                IS_TRUE(r.second == next(m.begin(), 8));
                r = m.equal_range(4);
                IS_TRUE(r.first == next(m.begin(), 0));
                IS_TRUE(r.second == next(m.begin(), 0));
                r = m.equal_range(6);
                IS_TRUE(r.first == next(m.begin(), 1));
                IS_TRUE(r.second == next(m.begin(), 1));
                r = m.equal_range(8);
                IS_TRUE(r.first == next(m.begin(), 2));
                IS_TRUE(r.second == next(m.begin(), 2));
                r = m.equal_range(10);
                IS_TRUE(r.first == next(m.begin(), 3));
                IS_TRUE(r.second == next(m.begin(), 3));
                r = m.equal_range(12);
                IS_TRUE(r.first == next(m.begin(), 4));
                IS_TRUE(r.second == next(m.begin(), 4));
                r = m.equal_range(14);
                IS_TRUE(r.first == next(m.begin(), 5));
                IS_TRUE(r.second == next(m.begin(), 5));
                r = m.equal_range(16);
                IS_TRUE(r.first == next(m.begin(), 6));
                IS_TRUE(r.second == next(m.begin(), 6));
                r = m.equal_range(18);
                IS_TRUE(r.first == next(m.begin(), 7));
                IS_TRUE(r.second == next(m.begin(), 7));
                r = m.equal_range(20);
                IS_TRUE(r.first == next(m.begin(), 8));
                IS_TRUE(r.second == next(m.begin(), 8));
            }
        }

        TEST_METHOD(TestFind)
        {
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                {
                    typedef M::iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.find(5);
                    IS_TRUE(r == m.begin());
                    r = m.find(6);
                    IS_TRUE(r == next(m.begin()));
                    r = m.find(7);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.find(8);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.find(9);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.find(10);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.find(11);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.find(12);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.find(4);
                    IS_TRUE(r == next(m.begin(), 8));
                }
                {
                    typedef M::const_iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    const M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.find(5);
                    IS_TRUE(r == m.begin());
                    r = m.find(6);
                    IS_TRUE(r == next(m.begin()));
                    r = m.find(7);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.find(8);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.find(9);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.find(10);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.find(11);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.find(12);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.find(4);
                    IS_TRUE(r == next(m.begin(), 8));
                }
            }
        }

        TEST_METHOD(TestLowerBound)
        {
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                {
                    typedef M::iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(7, 6),
                        V(9, 7),
                        V(11, 8),
                        V(13, 9),
                        V(15, 10),
                        V(17, 11),
                        V(19, 12)
                    };
                    M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.lower_bound(5);
                    IS_TRUE(r == m.begin());
                    r = m.lower_bound(7);
                    IS_TRUE(r == next(m.begin()));
                    r = m.lower_bound(9);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.lower_bound(11);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.lower_bound(13);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.lower_bound(15);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.lower_bound(17);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.lower_bound(19);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.lower_bound(4);
                    IS_TRUE(r == next(m.begin(), 0));
                    r = m.lower_bound(6);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.lower_bound(8);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.lower_bound(10);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.lower_bound(12);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.lower_bound(14);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.lower_bound(16);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.lower_bound(18);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.lower_bound(20);
                    IS_TRUE(r == next(m.begin(), 8));
                }
                {
                    typedef M::const_iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(7, 6),
                        V(9, 7),
                        V(11, 8),
                        V(13, 9),
                        V(15, 10),
                        V(17, 11),
                        V(19, 12)
                    };
                    const M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.lower_bound(5);
                    IS_TRUE(r == m.begin());
                    r = m.lower_bound(7);
                    IS_TRUE(r == next(m.begin()));
                    r = m.lower_bound(9);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.lower_bound(11);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.lower_bound(13);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.lower_bound(15);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.lower_bound(17);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.lower_bound(19);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.lower_bound(4);
                    IS_TRUE(r == next(m.begin(), 0));
                    r = m.lower_bound(6);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.lower_bound(8);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.lower_bound(10);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.lower_bound(12);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.lower_bound(14);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.lower_bound(16);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.lower_bound(18);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.lower_bound(20);
                    IS_TRUE(r == next(m.begin(), 8));
                }
            }
        }

        TEST_METHOD(TestUpperBound)
        {
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                {
                    typedef M::iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(7, 6),
                        V(9, 7),
                        V(11, 8),
                        V(13, 9),
                        V(15, 10),
                        V(17, 11),
                        V(19, 12)
                    };
                    M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.upper_bound(5);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.upper_bound(7);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.upper_bound(9);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.upper_bound(11);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.upper_bound(13);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.upper_bound(15);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.upper_bound(17);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.upper_bound(19);
                    IS_TRUE(r == next(m.begin(), 8));
                    r = m.upper_bound(4);
                    IS_TRUE(r == next(m.begin(), 0));
                    r = m.upper_bound(6);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.upper_bound(8);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.upper_bound(10);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.upper_bound(12);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.upper_bound(14);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.upper_bound(16);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.upper_bound(18);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.upper_bound(20);
                    IS_TRUE(r == next(m.begin(), 8));
                }
                {
                    typedef M::const_iterator R;
                    V ar[] =
                    {
                        V(5, 5),
                        V(7, 6),
                        V(9, 7),
                        V(11, 8),
                        V(13, 9),
                        V(15, 10),
                        V(17, 11),
                        V(19, 12)
                    };
                    const M m(ar, ar + sizeof(ar) / sizeof(ar[0]));
                    R r = m.upper_bound(5);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.upper_bound(7);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.upper_bound(9);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.upper_bound(11);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.upper_bound(13);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.upper_bound(15);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.upper_bound(17);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.upper_bound(19);
                    IS_TRUE(r == next(m.begin(), 8));
                    r = m.upper_bound(4);
                    IS_TRUE(r == next(m.begin(), 0));
                    r = m.upper_bound(6);
                    IS_TRUE(r == next(m.begin(), 1));
                    r = m.upper_bound(8);
                    IS_TRUE(r == next(m.begin(), 2));
                    r = m.upper_bound(10);
                    IS_TRUE(r == next(m.begin(), 3));
                    r = m.upper_bound(12);
                    IS_TRUE(r == next(m.begin(), 4));
                    r = m.upper_bound(14);
                    IS_TRUE(r == next(m.begin(), 5));
                    r = m.upper_bound(16);
                    IS_TRUE(r == next(m.begin(), 6));
                    r = m.upper_bound(18);
                    IS_TRUE(r == next(m.begin(), 7));
                    r = m.upper_bound(20);
                    IS_TRUE(r == next(m.begin(), 8));
                }
            }
        }

        TEST_METHOD(TestMemberSwap)
        {
            {
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                {
                    M m1;
                    M m2;
                    M m1_save = m1;
                    M m2_save = m2;
                    m1.swap(m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar2[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m1;
                    M m2(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]));
                    M m1_save = m1;
                    M m2_save = m2;
                    m1.swap(m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar1[] =
                    {
                        V(1, 1),
                        V(2, 2),
                        V(3, 3),
                        V(4, 4)
                    };
                    M m1(ar1, ar1 + sizeof(ar1) / sizeof(ar1[0]));
                    M m2;
                    M m1_save = m1;
                    M m2_save = m2;
                    m1.swap(m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar1[] =
                    {
                        V(1, 1),
                        V(2, 2),
                        V(3, 3),
                        V(4, 4)
                    };
                    V ar2[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m1(ar1, ar1 + sizeof(ar1) / sizeof(ar1[0]));
                    M m2(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]));
                    M m1_save = m1;
                    M m2_save = m2;
                    m1.swap(m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
            }
        }

        TEST_METHOD(TestNonmemberSwap)
        {
            {
                using lai::swap;
                typedef std::pair<const int, double> V;
                typedef MAP<int, double> M;
                {
                    M m1;
                    M m2;
                    M m1_save = m1;
                    M m2_save = m2;
                    swap(m1, m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar2[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m1;
                    M m2(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]));
                    M m1_save = m1;
                    M m2_save = m2;
                    swap(m1, m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar1[] =
                    {
                        V(1, 1),
                        V(2, 2),
                        V(3, 3),
                        V(4, 4)
                    };
                    M m1(ar1, ar1 + sizeof(ar1) / sizeof(ar1[0]));
                    M m2;
                    M m1_save = m1;
                    M m2_save = m2;
                    swap(m1, m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    V ar1[] =
                    {
                        V(1, 1),
                        V(2, 2),
                        V(3, 3),
                        V(4, 4)
                    };
                    V ar2[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m1(ar1, ar1 + sizeof(ar1) / sizeof(ar1[0]));
                    M m2(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]));
                    M m1_save = m1;
                    M m2_save = m2;
                    swap(m1, m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                }
                {
                    using C = TestGreater<int>;
                    typedef std::map<int, double, C> M;
                    V ar1[] =
                    {
                        V(1, 1),
                        V(2, 2),
                        V(3, 3),
                        V(4, 4)
                    };
                    V ar2[] =
                    {
                        V(5, 5),
                        V(6, 6),
                        V(7, 7),
                        V(8, 8),
                        V(9, 9),
                        V(10, 10),
                        V(11, 11),
                        V(12, 12)
                    };
                    M m1(ar1, ar1 + sizeof(ar1) / sizeof(ar1[0]), C(1));
                    M m2(ar2, ar2 + sizeof(ar2) / sizeof(ar2[0]), C(2));
                    M m1_save = m1;
                    M m2_save = m2;
                    swap(m1, m2);
                    IS_TRUE(m1 == m2_save);
                    IS_TRUE(m2 == m1_save);
                    IS_TRUE(m1.key_comp() == C(2));
                    IS_TRUE(m2.key_comp() == C(1));
                }
            }
        }
    };
}