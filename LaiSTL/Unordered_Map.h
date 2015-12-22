#pragma once
#include "Details\HashTable.h"
#include <functional>
#include <memory>
#include <utility>

namespace lai
{
    template<typename TKey,
        typename TValue,
        typename Hash,
        typename KeyEqual,
        typename Allocator>
    struct UMap_HashTraits
    {
        using key_type       = TKey;
        using value_type     = std::pair<const TKey, TValue>;
        using hasher         = Hash;
        using key_equal      = KeyEqual;
        using allocator_type = Allocator;
        using IsMulti        = details::FalseType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value.first;
        }
    };


    template<typename TKey,
        typename TValue,
        typename Hash = std::hash<TKey>,
        typename KeyEqual = std::equal_to<TKey>,
        typename Allocator = std::allocator<std::pair<const TKey, TValue>>>
    class unordered_map :
        public details::HashTable<UMap_HashTraits<TKey, TValue, Hash, KeyEqual, Allocator>>
    {
    private:
        using MyBase = details::HashTable<UMap_HashTraits<TKey, TValue, Hash, KeyEqual, Allocator>>;
    public:
        using mapped_type = TValue;
        using MyBase::HashTable;
    };


    template<typename TKey,
        typename TValue,
        typename Hash,
        typename KeyEqual,
        typename Allocator>
    struct UMultiMap_HashTraits
    {
        using key_type       = TKey;
        using value_type     = std::pair<const TKey, TValue>;
        using hasher         = Hash;
        using key_equal      = KeyEqual;
        using allocator_type = Allocator;
        using IsMulti        = details::TrueType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value.first;
        }
    };


    template<typename TKey,
        typename TValue,
        typename Hash = std::hash<TKey>,
        typename KeyEqual = std::equal_to<TKey>,
        typename Allocator = std::allocator<std::pair<const TKey, TValue>>>
    class unordered_multimap :
        public details::HashTable<UMultiMap_HashTraits<TKey, TValue, Hash, KeyEqual, Allocator>>
    {
    private:
        using MyBase = details::HashTable<
            UMultiMap_HashTraits<TKey, TValue, Hash, KeyEqual, Allocator>>;
    public:
        using mapped_type = TValue;
        using MyBase::HashTable;
    };
}