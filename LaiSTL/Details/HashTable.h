#pragma once

#include "../Vector.h"
#include "CommonTypes.h"
#include <iterator>
#include <initializer_list>
#include <type_traits>
#include <utility>

#define TRY_BEGIN try {
#define TRY_END }
#define CATCH_ALL_BEGIN catch(...) {
#define CATCH_ALL_END }

namespace lai
{
    namespace details
    {
        template<typename TValue>
        struct HashNode
        {
            using NodePtr = HashNode*;
            NodePtr next;   // must be the first field
            std::size_t hashCode;
            TValue value;
        };

        template<
            typename THashTable,
            typename TReference,
            typename TPointer,
            typename NodePtr>
        class HashTableIterator
        {
            friend THashTable;
        public:
            using value_type        = typename THashTable::value_type;
            using difference_type   = typename THashTable::difference_type;
            using pointer           = TPointer;
            using reference         = TReference;
            using iterator_category = std::forward_iterator_tag;
        private:
            NodePtr myNode = NodePtr();
        public:
            HashTableIterator() = default;

            HashTableIterator(NodePtr node) :
                myNode(node) { }

            // Convertion from iterator to const_iterator
            operator HashTableIterator<THashTable,
                typename THashTable::const_reference,
                typename THashTable::const_pointer,
                NodePtr>() const
            {
                return HashTableIterator<THashTable,
                    typename THashTable::const_reference,
                    typename THashTable::const_pointer,
                    NodePtr>(myNode);
            }

            reference operator*() const
            {
                return myNode->value;
            }

            pointer operator->() const
            {
                return std::addressof(operator*());
            }

            HashTableIterator & operator++()
            {
                myNode = myNode->next;
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
                return lhs.myNode == rhs.myNode;
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
            constexpr static float DEFAULT_MAX_LOAD_FACTOR    = 1.0F;
            constexpr static std::size_t DEFAULT_BUCKET_COUNT = 8;  // must be power of 2
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
            using Node         = HashNode<value_type>;
            using NodePtr      = Node*;
            using Bucket       = NodePtr;
            using NodeAlloc    = typename allocator_type::template rebind<Node>::other;
            using BucketAlloc  = typename allocator_type::template rebind<Bucket>::other;
            using BucketVector = lai::vector<Bucket, BucketAlloc>;
            static NodeAlloc nodeAlloc;
        public:
            using const_iterator = HashTableIterator<HashTable, const_reference, const_pointer, NodePtr>;
            using iterator       = SelectType<std::is_same<key_type, value_type>::value,
                                    const_iterator,
                                    HashTableIterator<HashTable, reference, pointer, NodePtr>>;
            using local_iterator       = iterator;
            using const_local_iterator = const_iterator;
            using PairIb               = std::pair<iterator, bool>;
            using PairIi               = std::pair<iterator, iterator>;
            using PairCc               = std::pair<const_iterator, const_iterator>;

        private:
            BucketVector buckets;
            NodePtr myHead;
            size_type mySize;
            float maxLoadFactor;
            hasher getHashCode;
            key_equal isKeyEqual;

        public:
            // constructor
            //////////////
            HashTable(size_type bucketCount, const hasher & hash, const key_equal & keyEqual) :
                buckets(bucketCount),
                mySize(0),
                myHead(nullptr),
                maxLoadFactor(DEFAULT_MAX_LOAD_FACTOR),
                getHashCode(hash),
                isKeyEqual(keyEqual) { }

            HashTable(size_type bucketCount, const hasher & hash) :
                HashTable(bucketCount, hash, key_equal()) { }

            explicit HashTable(size_type bucketCount) :
                HashTable(bucketCount, hasher()) { }

            HashTable() :
                HashTable(DEFAULT_BUCKET_COUNT) { }

            ~HashTable()
            {
                clear();
            }

            // iterators
            ////////////

            iterator begin() noexcept
            {
                return iterator(myHead);
            }

            const_iterator begin() const noexcept
            {
                return const_iterator(myHead);
            }

            const_iterator cbegin() const noexcept
            {
                return begin();
            }

            iterator end() noexcept
            {
                return iterator(nullptr);
            }

            const_iterator end() const noexcept
            {
                return const_iterator(nullptr);
            }

            const_iterator cend() const noexcept
            {
                return end();
            }

            local_iterator begin(size_type bucketIndex) noexcept
            {
                return buckets[bucketIndex] == nullptr ?
                    local_iterator(nullptr) :
                    local_iterator(firstNode(buckets[bucketIndex]));
            }

            const_local_iterator begin(size_type bucketIndex) const noexcept
            {
                return buckets[bucketIndex] == nullptr ?
                    const_local_iterator(nullptr) :
                    const_local_iterator(firstNode(buckets[bucketIndex]));
            }

            const_local_iterator cbegin(size_type bucketIndex) const noexcept
            {
                return begin(bucketIndex);
            }

            local_iterator end(size_type bucketIndex) noexcept
            {
                if (buckets[bucketIndex] == nullptr)
                    return local_iterator(nullptr);

                NodePtr node = firstNode(buckets[bucketIndex]);
                while (node != nullptr &&
                    getBucketIndex(node->hashCode, bucket_count()) == bucketIndex)
                {
                    node = node->next;
                }
                return local_iterator(node);
            }

            const_local_iterator end(size_type bucketIndex) const noexcept
            {
                return static_cast<const_local_iterator>(
                    const_cast<HashTable *>(this)->end(bucketIndex));
            }

            const_local_iterator cend(size_type bucketIndex) const noexcept
            {
                return end(bucketIndex);
            }

            // capacity
            ///////////

            size_type size() const noexcept
            {
                return mySize;
            }

            bool empty() const noexcept
            {
                return mySize == 0;
            }

            // modifiers
            ////////////

            void clear() noexcept
            {
                for (NodePtr next; myHead != nullptr; myHead = next)
                {
                    next = myHead->next;
                    deallocateNode(myHead);
                }
                buckets.clear();
                mySize = 0;
            }

            PairIb insert(const value_type & value)
            {
                return insertHelper(value, Nil());
            }

            PairIb insert(value_type && value)
            {
                return insertHelper(std::move(value), Nil());
            }

            template<typename InputIt,
                typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
            void insert(InputIt first, InputIt last)
            {
                while (first != last)
                {
                    insert(*first++);
                }
            }

            void insert(std::initializer_list<value_type> iList)
            {
                insert(iList.begin(), iList.end());
            }

            template<typename ... Args>
            PairIb emplace(Args && ... args)
            {
                NodePtr node = createNode(std::forward<Args>(args)...);
                return insertHelper(node->value, node);
            }

            iterator erase(const_iterator pos) noexcept
            {
                NodePtr node = pos.myNode;
                NodePtr ret = nullptr;
                auto bucketIndex = getBucketIndex(node->hashCode, bucket_count());
                NodePtr prev = buckets[bucketIndex];
                NodePtr curr = firstNode(buckets[bucketIndex]);
                while (curr != nullptr &&
                    getBucketIndex(curr->hashCode, bucket_count()) == bucketIndex)
                {
                    if (node == curr)
                    {
                        prev->next = curr->next;
                        if (node->next != nullptr &&
                            getBucketIndex(node->next->hashCode, bucket_count()) != bucketIndex)
                        {   // erased node is the before head of other bucket
                            auto otherBucketIndex = getBucketIndex(node->next->hashCode, bucket_count());
                            buckets[otherBucketIndex] = prev;
                        }

                        deallocateNode(node);
                        --mySize;
                        ret = prev->next;
                    }
                    prev = curr;
                    curr = curr->next;
                }
                return iterator(ret);
            }

            // bucket interface
            ///////////////////

            size_type bucket_count() const
            {
                return buckets.size();
            }

            size_type bucket_size(size_type bucketIndex) const
            {
                return std::distance(begin(bucketIndex), end(bucketIndex));
            }

            size_type bucket(const key_type & key) const
            {
                return getBucketIndex(getHashCode(key), bucket_count());
            }

            // Hash policy
            //////////////

            float load_factor() const
            {
                return size() / static_cast<float>(bucket_count());
            }

            float max_load_factor() const
            {
                return maxLoadFactor;
            }

            void max_load_factor(float newMaxLoadFactor)
            {
                if (newMaxLoadFactor < load_factor())
                {
                    size_type newBucketCount = size() / newMaxLoadFactor;
                    rehash(newBucketCount);
                }
                maxLoadFactor = newMaxLoadFactor;
            }

            void rehash(size_type newBucketCount)
            {
                if (newBucketCount == bucket_count())
                {
                    return;
                }
                // adjust bucket count to power of 2
                size_type realBucketCount = DEFAULT_BUCKET_COUNT;
                while (realBucketCount < newBucketCount)
                {
                    realBucketCount *= 2;
                }
                while (realBucketCount < size() / max_load_factor())
                {
                    realBucketCount *= 2;
                }
                // rehash
                buckets.assign(realBucketCount, nullptr);
                NodePtr prev = beforeHead();
                NodePtr curr = prev->next;
                while (curr != nullptr)
                {
                    size_type currHash = getBucketIndex(curr->hashCode, realBucketCount);
                    if (buckets[currHash] == nullptr)
                    {   // bucket is empty, put prev in bucket
                        buckets[currHash] = prev;
                        prev = curr;
                        curr = curr->next;
                    }
                    else
                    {   // bucket is NOT empty, put curr at head of bucket
                        NodePtr node = curr;
                        while (node->next != nullptr && isNodesEqual(node, node->next))
                        {
                            node = node->next;
                        }
                        prev->next = node->next;
                        node->next = firstNode(buckets[currHash]);
                        firstNode(buckets[currHash]) = curr;
                        curr = prev->next;
                    }
                }
            }

            void reserve(size_type newBucketCount)
            {
                // todo : reserve
            }

            hasher hash_function() const
            {
                return getHashCode;
            }

            key_equal key_eq() const
            {
                return isKeyEqual;
            }

        private:

            NodePtr allocateNode(size_type count)
            {
                return nodeAlloc.allocate(count);
            }

            void deallocateNode(NodePtr node)
            {
                nodeAlloc.destroy(std::addressof(node->value));
                nodeAlloc.deallocate(node, 1);
            }

            void deallocateNodeIfNotNil(NodePtr node)
            {
                deallocateNode(node);
            }

            void deallocateNodeIfNotNil(Nil)
            {
                /*empty*/
            }

            template<typename ... Args>
            NodePtr createNode(Args && ... args)
            {
                NodePtr newNode = allocateNode(1);
                try
                {
                    nodeAlloc.construct(
                        std::addressof(newNode->value), std::forward<Args>(args)...);
                    newNode->hashCode = getHashCode(MyTraits::getKey(newNode->value));
                }
                catch (...)
                {
                    nodeAlloc.deallocate(newNode, 1);
                    throw;
                }
                newNode->next = nullptr;
                return newNode;
            }

            template<typename TValue>
            NodePtr createNodeIfNil(TValue &&, NodePtr node)
            {
                return node;
            }

            template<typename TValue>
            NodePtr createNodeIfNil(TValue && value, Nil)
            {
                return createNode(std::forward<TValue>(value));
            }

        private:

            NodePtr beforeHead() noexcept
            {
                return (NodePtr)std::addressof(myHead);
            }

            NodePtr & firstNode(Bucket aBucket) const noexcept
            {
                return aBucket->next;
            }

            size_type getBucketIndex(size_type hashCode, size_type bucketCount) const noexcept
            {
                return hashCode & mask(bucketCount);
            }

            size_type mask(size_type bucketCount) const noexcept
            {
                return bucketCount - 1;
            }

            bool isNodesEqual(NodePtr lhs, NodePtr rhs) const noexcept
            {
                return isValuesEqual(lhs->value, rhs->value);
            }

            bool isValuesEqual(const value_type & lhs, const value_type & rhs) const noexcept
            {
                return isKeyEqual(MyTraits::getKey(lhs), MyTraits::getKey(rhs));
            }

            template<typename TValue, typename TNode>
            PairIb insertHelper(TValue && value, TNode aNode)
            {
                NodePtr node = nullptr;
                bool inserted = true;
                TRY_BEGIN
                    size_type hashCode = getHashCode(MyTraits::getKey(value));
                    size_type bucketIndex = getBucketIndex(hashCode, bucket_count());
                    if (buckets[bucketIndex] == nullptr)
                    {   // bucket is empty, insert at the head of element list
                        node = createNodeIfNil(std::forward<TValue>(value), aNode);
                        node->next = myHead;
                        if (myHead != nullptr)
                        {   // fixup previous head
                            size_type index = getBucketIndex(myHead->hashCode, bucket_count());
                            buckets[index] = node;
                        }
                        myHead = node;
                        buckets[bucketIndex] = beforeHead();
                        ++mySize;
                    }
                    else
                    {   // bucket is occupied
                        bool isKeyUnique = true;
                        NodePtr first = firstNode(buckets[bucketIndex]);
                        NodePtr prev = buckets[bucketIndex];
                        while (first != nullptr && getBucketIndex(first->hashCode, bucket_count()) == bucketIndex)
                        {   // check if key is unique
                            if (isValuesEqual(first->value, value))
                            {
                                isKeyUnique = false;
                                break;
                            }
                            prev = first;
                            first = first->next;
                        }

                        if (isKeyUnique)
                        {   // key is unique, insert at the head of bucket
                            node = createNodeIfNil(std::forward<TValue>(value), aNode);
                            node->next = firstNode(buckets[bucketIndex]);
                            firstNode(buckets[bucketIndex]) = node;
                            ++mySize;
                        }
                        else if (std::is_same<IsMulti, TrueType>::value)
                        {   // key is duplicated and IsMulti is true
                            // insert before the first equvalent key
                            node = createNodeIfNil(std::forward<TValue>(value), aNode);
                            node->next = first;
                            prev->next = node;
                            ++mySize;
                        }
                        else
                        {
                            inserted = false;
                        }
                    }
                TRY_END
                CATCH_ALL_BEGIN
                    deallocateNodeIfNotNil(aNode);
                    throw;
                CATCH_ALL_END


                TRY_BEGIN
                    checkLoadFactor();
                TRY_END
                CATCH_ALL_BEGIN
                    erase(iterator(node));
                    throw;
                CATCH_ALL_END

                return{ iterator(node), inserted };
            }

            void checkLoadFactor()
            {
                if (max_load_factor() < load_factor())
                {
                    rehash(0);
                }
            }

        };  // template class HashTable

        template<typename HashTraits>
        typename HashTable<HashTraits>::NodeAlloc
            HashTable<HashTraits>::nodeAlloc;
    }
}



