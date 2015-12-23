#pragma once
#include <functional>

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