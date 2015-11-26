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
    }; // template struct Map_TreeTraits


    template<typename TKey,
        typename TValue,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<std::pair<const TKey, TValue>>>
    class map : public details::Tree<Map_TreeTraits<TKey, TValue, Compare, Allocator>>
    {
    private:
        using MyBase   = details::Tree<Map_TreeTraits<TKey, TValue, Compare, Allocator>>;
    public:
        using mapped_type = TValue;

    public:
        using MyBase::Tree;

        map & operator=(std::initializer_list<value_type> iList)
        {
            MyBase::operator=(iList);
            return *this;
        }

        mapped_type & at(const key_type & key)
        {
            iterator iter = find(key);
            if (iter == end())
            {
                throw std::out_of_range("Invalid map<TKey, TValue> key");
            }
            return iter->second;
        }

        const mapped_type & at(const key_type & key) const
        {
            return static_cast<const mapped_type &>(
                const_cast<map*>(this)->at(key));
        }

        mapped_type & operator[](const key_type & key)
        {
            return try_emplace(key).first->second;
        }

        mapped_type & operator[](key_type && key)
        {
            return try_emplace(std::move(key)).first->second;
        }

        template<typename ... Args>
        PairIb try_emplace(const key_type & key, Args && ... args)
        {
            return tryEmplaceHelper(key, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        PairIb try_emplace(key_type && key, Args && ... args)
        {
            return tryEmplaceHelper(std::move(key), std::forward<Args>(args)...);
        }

        template<typename ValueType>
        PairIb insert_or_assign(const key_type & key, ValueType && obj)
        {
            return insertOrAssignHelper(key, std::forward<ValueType>(obj));
        }

        template<typename ValueType>
        PairIb insert_or_assign(key_type && key, ValueType && obj)
        {
            return insertOrAssignHelper(std::move(key), std::forward<ValueType>(obj));
        }

    private:
        template<typename KeyType, typename ... Args>
        PairIb tryEmplaceHelper(KeyType && key, Args && ... args)
        {
            iterator iter = lower_bound(key);
            if (iter == end() || compareKeys(key, iter->first))
            {
                return PairIb{ emplace_hint(iter, std::piecewise_construct,
                                    std::forward_as_tuple(std::forward<KeyType>(key)),
                                    std::forward_as_tuple(std::forward<Args>(args)...)),
                               true };
            }
            return PairIb{ iter, false };
        }

        template<typename KeyType, typename ValueType>
        PairIb insertOrAssignHelper(KeyType && key, ValueType && arg)
        {
            iterator iter = lower_bound(key);
            if (iter == end() || compareKeys(key, iter->first))
            {
                return PairIb{ emplace_hint(iter, std::forward<KeyType>(key), 
                                    std::forward<ValueType>(arg)),
                                true };
            }
            iter->second = std::forward<ValueType>(arg);
            return PairIb{ iter, false };
        }
    };  // template class map

    template<typename TKey,
        typename TValue,
        typename TCompare,
        typename Allocator>
    inline void swap(
        map<TKey, TValue, TCompare, Allocator> & lhs,
        map<TKey, TValue, TCompare, Allocator> & rhs)
    {
        lhs.swap(rhs);
    }


    template<typename TKey, typename TValue, typename TCompare, typename Allocator>
    struct MultiMap_TreeTraits
    {
        using key_type       = TKey;
        using value_type     = std::pair<const TKey, TValue>;
        using key_compare    = TCompare;
        using allocator_type = Allocator;
        using IsMulti        = details::TrueType;

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
    };  // template struct MultiMap_TreeTraits


    template<typename TKey,
        typename TValue,
        typename Compare = std::less<TKey>,
        typename Allocator = std::allocator<std::pair<const TKey, TValue>>>
    class multimap :
        public details::Tree<MultiMap_TreeTraits<TKey, TValue, Compare, Allocator>>
    {
    private:
        using MyBase = details::Tree<MultiMap_TreeTraits<TKey, TValue, Compare, Allocator>>;
    public:
        using mapped_type = TValue;

    public:
        using MyBase::Tree;

        multimap & operator=(std::initializer_list<value_type> iList)
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
    };  // template class multimap

    template<typename TKey,
        typename TValue,
        typename TCompare,
        typename Allocator>
        inline void swap(
            multimap<TKey, TValue, TCompare, Allocator> & lhs,
            multimap<TKey, TValue, TCompare, Allocator> & rhs)
    {
        lhs.swap(rhs);
    }
}