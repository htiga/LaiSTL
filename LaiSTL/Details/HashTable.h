#pragma once

#include "../Forward_List.h"
#include <iterator>

namespace lai
{
    namespace details
    {
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
            using IsMulti = typename MyTraits::IsMulti;
        public:
            using allocator_type = typename MyTraits::allocator_type;
            using key_type = typename MyTraits::key_type;
            using value_type = typename MyTraits::value_type;
            using size_type = typename allocator_type::size_type;
            using difference_type = typename allocator_type::difference_type;
            using hasher = typename MyTraits::hasher;
            using key_equal = typename MyTraits::key_equal;
            using reference = typename allocator_type::reference;
            using const_reference = typename allocator_type::const_reference;
            using pointer = typename allocator_type::pointer;
            using const_pointer = typename allocator_type::const_pointer;
        private:
            using Node = HashNode<value_type>;
            using MyList = lai::forward_list<Node>;
            using ListIterator = typename MyList::iterator;
            using MyVector = lai::vector<ListIterator>;

        public:
            // bucket interface
            ///////////////////

            size_type bucket_count() const
            {
                // todo : bucket_count
            }

            size_type bucket_size(size_type bucketIndex) const
            {
                // todo : bucket_size
            }

            size_type bucket(const TKey & key) const
            {
                // todo : bucket
            }

            // Hash policy
            //////////////

            float load_factor() const
            {
                // todo : load_factor
            }

            float max_load_factor() const
            {
                // todo : max_load_factor
            }

            void max_load_factor(float newMaxLoadFactor)
            {
                // todo
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
                // todo : hash_function
            }

            key_equal key_eq() const
            {
                // todo : key_eq
            }
        };  // template class HashTable
    }
}
