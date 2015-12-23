#pragma once
#include <functional>

// labeled comparer for unit tests
template<typename Compare>
struct LabeledCompare : public Compare
{
    int id;

    explicit LabeledCompare(int i = 0) :
        Compare(), id(i) { }

    bool operator==(const LabeledCompare & rhs) const noexcept
    {
        return id == rhs.id;
    }

    bool operator!=(const LabeledCompare & rhs) const noexcept
    {
        return !(*this == rhs);
    }
};

template<typename TKey>
using LabeledGreater = LabeledCompare<std::greater<TKey>>;

template<typename TKey>
using LabeledLess = LabeledCompare<std::less<TKey>>;

template<typename TKey>
using LabeledEqualTo = LabeledCompare<std::equal_to<TKey>>;

template<typename TKey>
using LabeledHash = LabeledCompare<std::hash<TKey>>;