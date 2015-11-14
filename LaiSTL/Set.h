#pragma once

#include "./Details/CommonTypes.h"
#include "./Details/RB_Tree.h"
#include <memory>
#include <iterator>

namespace lai
{
    template<typename TKey, typename TCompare, typename Allocator>
    struct Set_TreeTraits
    {
        using key_type       = TKey;
        using value_type     = TKey;
        using key_compare    = TCompare;
        using value_compare  = TCompare;
        using allocator_type = Allocator;
        using IsMulti        = details::FalseType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value;
        }
    };

    template<typename TKey,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<TKey >>
    class set : public details::Tree<Set_TreeTraits<TKey, Compare, Allocator>>
    {
    private:
        using MyBase = details::Tree<Set_TreeTraits<TKey, Compare, Allocator>>;

    public:
        using MyBase::Tree;

        set() :
            set(key_compare()) { }

        set(const set & rhs) :
            MyBase(rhs) { }

        set(set && rhs) :
            MyBase(std::move(rhs)) { }

        set & operator=(const set & rhs)
        {
            MyBase::operator=(rhs);
            return *this;
        }

        set & operator=(set && rhs)
        {
            MyBase::operator=(std::move(rhs));
            return *this;
        }
    };


    template<typename TKey, typename TCompare, typename Allocator>
    struct MultiSet_TreeTraits
    {
        using key_type       = TKey;
        using value_type     = TKey;
        using key_compare    = TCompare;
        using value_compare  = TCompare;
        using allocator_type = Allocator;
        using IsMulti        = details::TrueType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value;
        }
    };


    template<typename TKey,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<TKey >>
    class multiset : public details::Tree<MultiSet_TreeTraits<TKey, Compare, Allocator>>
    {
    private:
        using MyBase = details::Tree<MultiSet_TreeTraits<TKey, Compare, Allocator>>;

    public:
        using MyBase::Tree;

        multiset() :
            multiset(key_compare()) { }

        multiset(const multiset & rhs) :
            MyBase(rhs) { }

        multiset(multiset && rhs) :
            MyBase(std::move(rhs)) { }

        multiset & operator=(const multiset & rhs)
        {
            MyBase::operator=(rhs);
            return *this;
        }

        multiset & operator=(multiset && rhs)
        {
            MyBase::operator=(std::move(rhs));
            return *this;
        }
    };
}