#pragma once

#include "../Forward_List.h"
#include "../Vector.h"
#include "CommonTypes.h"
#include <iterator>
#include <type_traits>

namespace lai
{
    namespace details
    {
        struct HashPolicys
        {
            constexpr static float DEFAULT_MAX_LOAD_FACTOR    = 1.0F;
            constexpr static std::size_t DEFAULT_BUCKET_COUNT = 8;
        };

        template<typename TValue>
        struct HashNode
        {
            std::size_t hashCode;
            TValue value;
        };

        template<
            typename THashTable,
            typename TReference,
            typename TPointer,
            typename FListIterator>
        class HashTableIterator
        {
        public:
            using value_type = typename THashTable::value_type;
            using difference_type = typename THashTable::difference_type;
            using pointer = TPointer;
            using reference = TReference;
            using iterator_category = std::forward_iterator_tag;
        private:
            FListIterator myFListIter = FListIterator();
        public:
            HashTableIterator() = default;

            HashTableIterator(FListIterator node) :
                myFListIter(node) { }

            // Convertion from iterator to const_iterator
            operator HashTableIterator<THashTable,
                typename THashTable::const_reference,
                typename THashTable::const_pointer,
                typename FListIterator>() const
            {
                return HashTableIterator<THashTable,
                    typename THashTable::reference,
                    typename THashTable::pointer,
                    FListIterator>(myFListIter);
            }

            reference operator*() const
            {
                return myFListIter->value;
            }

            pointer operator->() const
            {
                return std::addressof(operator*());
            }

            HashTableIterator & operator++()
            {
                ++myFListIter;
                return *this;
            }

            HashTableIterator operator++(int)
            {
                auto old = *this;
                ++*this;
                return old;
            }

            friend bool operator==(
                const HashTableIterator & lhs,
                const HashTableIterator & rhs) noexcept
            {
                return lhs.myFListIter == rhs.myFListIter;
            }

            friend bool operator!=(
                const HashTableIterator & lhs,
                const HashTableIterator & rhs) noexcept
            {
                return !(lhs == rhs);
            }
        };

        // HashTraits must contain following public members :
        //      typedefs : key_type
        //                 value_type 
        //                 hasher
        //                 key_equal
        //                 allocator_type
        //                 IsMulti (details::TrueType or details::FalseType)
        //      static const key_type & getKey(const value_type & value);
        template<typename HashTraits>
        class HashTable
        {
        protected:
            using MyTraits = HashTraits;
            using IsMulti  = typename MyTraits::IsMulti;
        public:
            using allocator_type  = typename MyTraits::allocator_type;
            using key_type        = typename MyTraits::key_type;
            using value_type      = typename MyTraits::value_type;
            using size_type       = typename allocator_type::size_type;
            using difference_type = typename allocator_type::difference_type;
            using hasher          = typename MyTraits::hasher;
            using key_equal       = typename MyTraits::key_equal;
            using reference       = typename allocator_type::reference;
            using const_reference = typename allocator_type::const_reference;
            using pointer         = typename allocator_type::pointer;
            using const_pointer   = typename allocator_type::const_pointer;
        private:
            // todo : proper allocator type for MyList and MyVector
            using Node         = HashNode<value_type>;
            using MyList       = lai::forward_list<Node>;
            using ListIterator = typename MyList::iterator;
            using MyVector     = lai::vector<ListIterator>;
        public:
            using const_iterator = HashTableIterator<HashTable, const_reference, const_pointer, ListIterator>;
            using iterator       = SelectType<std::is_same<key_type, value_type>::value,
                                    const_iterator,
                                    HashTableIterator<HashTable, reference, pointer, ListIterator>>;
            using PairIb = std::pair<iterator, bool>;
            using PairIi = std::pair<iterator, iterator>;
            using PairCc = std::pair<const_iterator, const_iterator>;

        private:
            MyList elements;
            MyVector buckets;
            size_type mySize = 0;
            float maxLoadFactor = HashPolicys::DEFAULT_MAX_LOAD_FACTOR;
            hasher getHashValue;
            key_equal isKeyEqual;

        public:
            // capacity
            ///////////

            size_type size() const noexcept
            {
                return mySize;
            }

            // bucket interface
            ///////////////////

            size_type bucket_count() const
            {
                return buckets.size();
            }

            size_type bucket_size(size_type bucketIndex) const
            {
                // todo : bucket_size
            }

            size_type bucket(const TKey & key) const
            {
                return bucket_count() / static_cast<float>(size());
            }

            // Hash policy
            //////////////

            float load_factor() const
            {
                // todo : load_factor
            }

            float max_load_factor() const
            {
                return maxLoadFactor;
            }

            void max_load_factor(float newMaxLoadFactor)
            {
                // todo : set max_load_factor
            }

            void rehash(size_type newBucketCount)
            {
                // todo : rehash
            }

            void reserve(size_type newBucketCount)
            {
                // todo : reserve
            }

            hasher hash_function() const
            {
                return getHashValue;
            }

            key_equal key_eq() const
            {
                return isKeyEqual;
            }
        };  // template class HashTable
    }
}
