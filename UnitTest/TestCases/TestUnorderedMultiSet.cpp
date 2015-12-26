#include "stdafx.h"
#include "../LaiSTL/Unordered_Set.h"
#include <set>
#include <functional>

#define LAI_IUMSET lai::unordered_multiset<int>
#define LAI_SUMSET lai::unordered_multiset<std::string>
#define LAI_UUMSET lai::unordered_multiset<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestUMultiSet)
    {   
        TEST_METHOD(TestBraceListConstructor)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            C c(I_IL{ 1,2,3,4,1,2 }, 10, LabeledHash<int>(1), LabeledEqualTo<int>(2));

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>(2));
            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestBraceListConstructor1)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            C c(I_IL{ 1,2,3,4,1,2 }, 10, LabeledHash<int>(1));

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestBraceListConstructor2)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            C c(I_IL{ 1,2,3,4,1,2 }, 10);

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>());
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestBraceListConstructor3)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            C c(I_IL{ 1,2,3,4,1,2 });

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>());
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestRangeConstructor)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            I_IL data = { 1,2,3,4,1,2 };
            C c(data.begin(), data.end(), 10, LabeledHash<int>(1), LabeledEqualTo<int>(2));

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>(2));
            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestRangeConstructor1)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            I_IL data = { 1,2,3,4,1,2 };
            C c(data.begin(), data.end(), 10, LabeledHash<int>(1));

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>(1));
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestRangeConstructor2)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            I_IL data = { 1,2,3,4,1,2 };
            C c(data.begin(), data.end(), 10);

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>());
            IS_TRUE(c.bucket_count() >= 10);
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
            IS_TRUE(c.count(3) == 1);
            IS_TRUE(c.count(4) == 1);
            IS_TRUE(!c.empty());
            IS_TRUE(std::distance(c.begin(), c.end()) == c.size());
            IS_TRUE(std::distance(c.cbegin(), c.cend()) == c.size());
            IS_TRUE(fabs(c.load_factor() - (float)c.size() / c.bucket_count()) < FLT_EPSILON);
            IS_TRUE(c.max_load_factor() == 1);
        }

        TEST_METHOD(TestRangeConstructor3)
        {
            typedef lai::unordered_multiset<int, LabeledHash<int>, LabeledEqualTo<int>> C;

            I_IL data = { 1,2,3,4,1,2 };
            C c(data.begin(), data.end());

            IS_TRUE(c.key_eq() == LabeledEqualTo<int>());
            IS_TRUE(c.hash_function() == LabeledHash<int>());
            IS_TRUE(c.size() == 6);
            IS_TRUE(c.count(1) == 2);
            IS_TRUE(c.count(2) == 2);
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