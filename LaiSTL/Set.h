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

        set & operator=(std::initializer_list<value_type> iList)
        {
            MyBase::operator=(iList);
            return *this;
        }
    };


    template<typename TKey, typename TCompare, typename Allocator>
    inline void swap(
        set<TKey, TCompare, Allocator> & lhs, set<TKey, TCompare, Allocator> & rhs)
    {
        lhs.swap(rhs);
    }


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

        multiset & operator=(std::initializer_list<value_type> iList)
        {
            MyBase::operator=(iList);
            return *this;
        }

        using MyBase::insert;

        iterator insert(const value_type & value)
        {
            return MyBase::insert(value).first;
        }

        iterator insert(value_type && value)
        {
            return MyBase::insert(std::move(value)).first;
        }

        template<typename ... Args>
        iterator emplace(Args && ... args)
        {
            return MyBase::emplace(std::forward<Args>(args)...).first;
        }
    };


    template<typename TKey, typename TCompare, typename Allocator>
    inline void swap(
        multiset<TKey, TCompare, Allocator> & lhs, multiset<TKey, TCompare, Allocator> & rhs)
    {
        lhs.swap(rhs);
    }
}