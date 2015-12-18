#pragma once

#include "Details\HashTable.h"
#include <functional>

namespace lai
{
    template<
        typename TKey,
        typename Hash,
        typename KeyEqual,
        typename Allocator>
    struct USet_HashTraits
    {
        using key_type       = TKey;
        using value_type     = TKey;
        using hasher         = Hash;
        using key_equal      = KeyEqual;
        using allocator_type = Allocator;
        using IsMulti        = details::FalseType;

        static const key_type & getKey(const value_type & value) noexcept
        {
            return value;
        }
    };

    template<
        typename TKey,
        typename Hash = std::hash<TKey>,
        typename KeyEqual = std::equal_to<TKey>,
        typename Allocator = std::allocator<TKey>>
    class unordered_set :
        public details::HashTable<USet_HashTraits<TKey, Hash, KeyEqual, Allocator>>
    {
    private:
        using MyBase = details::HashTable<USet_HashTraits<TKey, Hash, KeyEqual, Allocator>>;
    public:
        using MyBase::HashTable;
    };
}