#pragma once

#include "./Details/RB_Tree.h"
#include <memory>
#include <iterator>

namespace lai
{
    template<typename TKey, typename TCompare, typename Allocator>
    struct Set_TreeTraits
    {
        using key_type = TKey;
        using value_type = TKey;
        using key_compare = TCompare;
        using value_compare = TCompare;
        using allocator_type = Allocator;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value;
        }

        constexpr static bool isMulti = false;
    };

    template<typename TKey,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<TKey >>
    class set : public details::Tree<Set_TreeTraits<TKey, Compare, Allocator>>
    {
    private:
        using MyBase = details::Tree<Set_TreeTraits<TKey, Compare, Allocator>>;

    public:
        explicit set(const key_compare & comp = key_compare()) :
            MyBase(comp) { }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
            set(InputIt first, InputIt last, const key_compare & comp = key_compare()) :
            MyBase(first, last, comp) { }

        set(std::initializer_list<value_type> iList, const key_compare & comp = key_compare()) :
            MyBase(iList, comp) { }
    };
}