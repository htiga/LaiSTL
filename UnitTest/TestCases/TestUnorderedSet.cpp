#include "stdafx.h"
#include "../LaiSTL/Unordered_Set.h"
#include <type_traits>
#include <functional>
#include <iterator>
#include <float.h>

#define LAI_USET lai::unordered_set

namespace UnitTest
{
    TEST_CLASS(TestUnorderedSet)
    {


        TEST_METHOD(TestTypeAlias)
        {
            // http://llvm.org/svn/llvm-project/libcxx/trunk/test/std/containers/unord/unord.set/types.pass.cpp
            typedef LAI_USET<int> C;
            static_assert((std::is_same<C::value_type, int>::value), "");
            static_assert((std::is_same<C::key_type, int>::value), "");
            static_assert((std::is_same<C::hasher, std::hash<C::key_type> >::value), "");
            static_assert((std::is_same<C::key_equal, std::equal_to<C::key_type> >::value), "");
            static_assert((std::is_same<C::allocator_type, std::allocator<C::value_type> >::value), "");
            static_assert((std::is_same<C::reference, C::value_type&>::value), "");
            static_assert((std::is_same<C::const_reference, const C::value_type&>::value), "");
            static_assert((std::is_same<C::pointer, C::value_type*>::value), "");
            static_assert((std::is_same<C::const_pointer, const C::value_type*>::value), "");
            static_assert((std::is_same<C::size_type, std::size_t>::value), "");
            static_assert((std::is_same<C::difference_type, std::ptrdiff_t>::value), "");
        }

        TEST_METHOD(TestDefaultContructor)
        {
            LAI_USET<int, LabeledHash<int>, LabeledEqualTo<int>> c;

            IS_TRUE(c.hash_function() == LabeledHash<int>());
            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.size() == 0);
            IS_TRUE(c.empty());
            IS_TRUE(c.load_factor() == 0.0F);
            IS_TRUE(c.max_load_factor() == 1.0F);
            IS_TRUE(std::distance(c.begin(), c.end()) == 0);
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == 0);
        }

        TEST_METHOD(TestContructorWithHash)
        {
            LAI_USET<int, LabeledHash<int>, LabeledEqualTo<int>> c(20, LabeledHash<int>(1));

            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.bucket_count() >= 20);
            IS_TRUE(c.size() == 0);
            IS_TRUE(c.empty());
            IS_TRUE(c.load_factor() == 0.0F);
            IS_TRUE(c.max_load_factor() == 1.0F);
            IS_TRUE(std::distance(c.begin(), c.end()) == 0);
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == 0);
        }

        TEST_METHOD(TestContructorWithKeyEqual)
        {
            LAI_USET<int, LabeledHash<int>, LabeledEqualTo<int>> c(
                11, LabeledHash<int>(1), LabeledEqualTo<int>(2));

            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.key_eq() == LabeledEqualTo<int>(2));
            IS_TRUE(c.bucket_count() >= 11);
            IS_TRUE(c.size() == 0);
            IS_TRUE(c.empty());
            IS_TRUE(c.load_factor() == 0.0F);
            IS_TRUE(c.max_load_factor() == 1.0F);
            IS_TRUE(std::distance(c.begin(), c.end()) == 0);
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == 0);
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            typedef lai::unordered_set<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            C c(I_IL{ 1,2,3,4,1,2 } , 10, LabeledHash<int>(1), LabeledEqualTo<int>(2));

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>(2));
            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 4);
            IS_TRUE(c.count(1) == 1);
            IS_TRUE(c.count(2) == 1);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }
    };
}