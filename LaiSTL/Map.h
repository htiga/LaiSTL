#pragma once

#include "./Details/CommonTypes.h"
#include "./Details/RB_Tree.h"
#include <utility>
#include <functional>
#include <initializer_list>

namespace lai
{
    template<typename TKey, typename TValue, typename TCompare, typename Allocator>
    struct Map_TreeTraits
    {
        using key_type       = TKey;
        using value_type     = std::pair<const TKey, TValue>;
        using key_compare    = TCompare;
        using allocator_type = Allocator;
        using IsMulti        = details::FalseType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value.first;
        }

        struct value_compare
        {
            using result_type          = bool;
            using first_argument_type  = typename Map_TreeTraits::value_type;
            using second_argument_type = typename Map_TreeTraits::value_type;

            bool operator()(const first_argument_type & lhs, const second_argument_type & rhs) const
            {
                return myComp(lhs.first, rhs.first);
            }

        protected:
            value_compare(TCompare comp) :
                myComp(comp) { }

            TCompare myComp;
        };
    };

    template<typename TKey,
        typename TValue,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<std::pair<const TKey, TValue>>>
    class map : public details::Tree<Map_TreeTraits<TKey, TValue, Compare, Allocator>>
    {
    private:
        using MyTraits = Map_TreeTraits<TKey, TValue, Compare, Allocator>;
        using MyBase   = details::Tree<Map_TreeTraits<TKey, TValue, Compare, Allocator>>;
    public:
        using mapped_type = TValue;

    public:
        using MyBase::Tree;

        map() :
            MyBase(key_compare()) { }

        map(const map & rhs) :
            MyBase(rhs) { }

        map(map && rhs) noexcept :
            MyBase(std::move(rhs)) { }

        map & operator=(const map & rhs)
        {
            MyBase::operator=(rhs);
            return *this;
        }

        map & operator=(map && rhs) noexcept
        {
            MyBase::operator=(rhs);
            return *this;
        }

        map & operator=(std::initializer_list<value_type> iList)
        {
            MyBase::operator=(iList);
            return *this;
        }
    };
}