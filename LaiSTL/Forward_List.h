#pragma once

#include <memory>
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <functional>

namespace lai
{
    template<typename ValueType>
    struct FListNode
    {
        // To support before_begin, next must be the before_first field.
        // Then before_begin shall be the address of before_first node.
        FListNode * next;
        ValueType value;
    };

    template<typename TFList, typename TReference, typename TPointer>
    class FListIterator
    {
        friend TFList;
    public:
        using value_type        = typename TFList::value_type;
        using difference_type   = typename TFList::difference_type;
        using pointer           = TPointer;
        using reference         = TReference;
        using iterator_category = std::forward_iterator_tag;

    private:
        using NodePtr = FListNode<value_type> *;
        NodePtr myNode = NodePtr();

    public:
        FListIterator() = default;

        FListIterator(NodePtr aNode) :
            myNode(aNode) { }

        operator FListIterator<TFList,
            typename TFList::const_reference,
            typename TFList::const_pointer>() const
        {
            return FListIterator<TFList,
                typename TFList::const_reference,
                typename TFList::const_pointer>(myNode);
        }

        FListIterator & operator++()
        {
            myNode = myNode->next;
            return *this;
        }

        FListIterator operator++(int)
        {
            FListIterator old = *this;
            ++*this;
            return old;
        }

        bool operator==(const FListIterator & rhs) const noexcept
        {
            return myNode == rhs.myNode;
        }

        bool operator!=(const FListIterator & rhs) const noexcept
        {
            return !(*this == rhs);
        }

        reference operator*() const
        {
            return myNode->value;
        }

        pointer operator->() const
        {
            return std::addressof(operator*());
        }
    }; // template class FListIterator


    template<typename T, typename Allocator = std::allocator<T>>
    class forward_list
    {
    private:
        using Node      = FListNode<T>;
        using NodePtr   = Node *;
        using NodeAlloc = typename Allocator::template rebind<Node>::other;

        static NodeAlloc nodeAlloc;
        static Allocator dataAlloc;
        NodePtr myHead = NodePtr();

    public:
        using value_type      = T;
        using allocator_type  = Allocator;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference       = value_type &;
        using const_reference = const value_type &;
        using pointer         = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator        = FListIterator<forward_list, reference, pointer>;
        using const_iterator  = FListIterator<forward_list, const_reference, const_pointer>;

        forward_list() = default;

        forward_list(size_type count, const T & value)
        {
            insertAfterN(before_begin(), count, value);
        }

        explicit forward_list(size_type count)
        {
            insertAfterN(before_begin(), count);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
            forward_list(InputIt first, InputIt last)
        {
            insertAfterRange(before_begin(), first, last);
        }

        forward_list(const forward_list & rhs)
        {
            insertAfterRange(before_begin(), rhs.begin(), rhs.end());
        }

        forward_list(forward_list && rhs) :
            myHead(std::move(rhs.myHead))
        {
            rhs.myHead = NodePtr();
        }

        forward_list(std::initializer_list<T> init)
        {
            insertAfterRange(before_begin(), init.begin(), init.end());
        }

        ~forward_list()
        {
            clear();
        }

        forward_list & operator=(const forward_list & rhs)
        {
            if (std::addressof(rhs) != this)
            {
                forward_list copied = rhs;
                swap(copied);
            }
            return *this;
        }

        forward_list & operator=(forward_list && rhs)
        {
            forward_list copied = std::move(rhs);
            swap(copied);
            return *this;
        }

        forward_list & operator=(std::initializer_list<T> iList)
        {
            forward_list copied(iList.begin(), iList.end());
            swap(copied);
            return *this;
        }

        void assign(size_type count, const T & value)
        {
            swap(forward_list(count, value));
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
        void assign(InputIt first, InputIt last)
        {
            swap(forward_list(first, last));
        }

        void assign(std::initializer_list<T> iList)
        {
            swap(forward_list(iList));
        }

        // element access

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *begin();
        }

        // iterators

        iterator before_begin()
        {
            return iterator((NodePtr)std::addressof(myHead));
        }

        const_iterator before_begin() const
        {
            return const_iterator((NodePtr)std::addressof(myHead));
        }

        const_iterator cbefore_begin() const
        {
            return before_begin();
        }

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

        // capacity

        bool empty() const noexcept
        {
            return myHead == NodePtr();
        }

        size_type size() const noexcept
        {
            return std::distance(begin(), end());
        }

        // modifiers

        void clear() noexcept
        {
            destroyNodeRange(myHead, NodePtr());
            myHead = NodePtr();
        }

        iterator insert_after(const_iterator before_pos, const T & value)
        {
            return emplace_after(before_pos, value);
        }

        iterator insert_after(const_iterator before_pos, T && value)
        {
            return emplace_after(before_pos, std::move(value));
        }

        iterator insert_after(const_iterator before_pos, size_type count, const T & value)
        {
            return insertAfterN(before_pos, count, value);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        iterator insert_after(const_iterator before_pos, InputIt first, InputIt last)
        {
            return insertAfterRange(before_pos, first, last);
        }

        iterator insert_after(const_iterator before_pos, std::initializer_list<T> iList)
        {
            return insertAfterRange(before_pos, iList.begin(), iList.end());
        }

        template<typename ... Args>
        iterator emplace_after(const_iterator before_pos, Args && ... args)
        {
            NodePtr node = before_pos.myNode;
            NodePtr newNode = createNode(node->next, std::forward<Args>(args)...);
            node->next = newNode;
            return iterator(newNode);
        }

        iterator erase_after(const_iterator pos)
        {
            NodePtr node = pos.myNode->next;
            pos.myNode->next = node->next;
            destrotyNode(node);
            return iterator(pos.myNode->next);
        }

        iterator erase_after(const_iterator first, const_iterator last)
        {   // remove elements in (before_first, last)
            NodePtr firstNode = first.myNode;
            NodePtr lastNode = last.myNode;
            NodePtr start = firstNode->next;
            firstNode->next = lastNode;
            destroyNodeRange(start, lastNode);
            return iterator(lastNode);
        }

        void push_front(const T & value)
        {
            emplace_after(before_begin(), value);
        }

        void push_front(T && value)
        {
            emplace_after(before_begin(), std::move(value));
        }

        template<typename ... Args>
        void emplace_front(Args && ... args)
        {
            emplace_after(before_begin(), std::forward<Args>(args)...);
        }

        void pop_front()
        {
            erase_after(before_begin());
        }

        void resize(size_type count)
        {
            size_type mySize = std::distance(begin(), end());
            if (count < mySize)
            {
                auto start = before_begin();
                while (count-- > 0)
                {
                    ++start;
                }
                erase_after(start, end());
            }
            else if (count > mySize)
            {
                insertAfterN(before_end(), count - mySize);
            }
        }

        void resize(size_type count, const T & value)
        {
            size_type mySize = std::distance(begin(), end());
            if (count < mySize)
            {
                auto start = before_begin();
                while (count-- > 0)
                {
                    ++start;
                }
                erase_after(start, end());
            }
            else if (count > mySize)
            {
                insertAfterN(before_end(), count - mySize, value);
            }
        }

        void swap(forward_list & rhs) noexcept
        {
            using std::swap;
            swap(myHead, rhs.myHead);
        }

        // operations

        void merge(forward_list & rhs)
        {
            merge(rhs, std::less<>());
        }

        template<typename Compare>
        void merge(forward_list & rhs, Compare comp)
        {
            if (this == &rhs || rhs.empty())
            {
                return;
            }

            iterator self = before_begin();
            iterator selfNext = begin();
            iterator selfEnd = end();

            iterator other = rhs.before_begin();
            iterator otherNext = rhs.begin();
            iterator otherEnd = rhs.end();

            for (/* empty */; selfNext != selfEnd && otherNext != otherEnd; ++self)
            {
                if (comp(*otherNext, *selfNext))
                {
                    splice_after(self, rhs, other);
                    otherNext = std::next(other);
                }
                else
                {
                    ++selfNext;
                }
            }
            if (otherNext != otherEnd)
            {
                splice_after(self, rhs, other, otherEnd);
            }
        }

        void splice_after(const_iterator before_pos, forward_list & rhs)
        {
            spliceAfter(before_pos, rhs, rhs.before_begin(), rhs.end());
        }

        void splice_after(const_iterator before_pos, forward_list & rhs, const_iterator before_iter)
        {
            if (before_pos == before_iter || before_pos == std::next(before_iter))
            {
                return;
            }
            spliceAfter(before_pos, rhs, before_iter, std::next(before_iter, 2));
        }

        void splice_after(const_iterator before_pos, forward_list & rhs,
                          const_iterator before_first, const_iterator last)
        { 
            spliceAfter(before_pos, rhs, before_first, last);
        }

        void remove(const T & value)
        {
            remove_if([&value](const T & arg) { return arg == value; });
        }

        template<typename UnaryPredicate>
        void remove_if(UnaryPredicate pred)
        {
            auto prev = before_begin();
            for(auto iter = begin(); iter != end(); /* empty */)
            {
                if (pred(*iter))
                {
                    iter = erase_after(prev);
                }
                else
                {
                    ++iter;
                    ++prev;
                }
            }
        }

        void reverse()
        {
            NodePtr prev = nullptr;
            NodePtr curr = begin().myNode;
            while (curr != nullptr)
            {
                NodePtr next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
            }
            myHead = prev;
        }

        void unique()
        {
            return unique(std::equal_to<>());
        }

        template<typename BinaryPredicate>
        void unique(BinaryPredicate pred)
        {
            if (!empty())
            {
                auto curr = begin();
                for (auto next = ++begin(); next != end(); /* empty */)
                {
                    if (pred(*curr, *next))
                    {
                        next = erase_after(curr);
                    }
                    else
                    {
                        ++curr;
                        ++next;
                    }
                }
            }
        }

        void sort()
        {
            return sort(std::less<>());
        }

        template<typename Compare>
        void sort(Compare comp)
        {
            mergeSort(before_begin(), end(), size(), comp);
        }

    private:

        template<typename ... Args>
        static NodePtr createNode(NodePtr next, Args && ... args)
        {
            NodePtr newNode = nodeAlloc.allocate(1);
            try
            {
                dataAlloc.construct(std::addressof(newNode->value),
                    std::forward<Args>(args)...);
            }
            catch (...)
            {
                nodeAlloc.deallocate(newNode, 1);
                throw;
            }
            newNode->next = next;
            return newNode;
        }

        static void destrotyNode(NodePtr node)
        {
            dataAlloc.destroy(std::addressof(node->value));
            nodeAlloc.deallocate(node, 1);
        }

        // destroy nodes in [start, finish)
        static void destroyNodeRange(NodePtr start, NodePtr finish)
        {
            while (start != finish)
            {
                NodePtr next = start->next;
                destrotyNode(start);
                start = next;
            }
        }

        iterator before_end() noexcept
        {
            auto ret = before_begin();
            for (const auto & _ : *this)
            {
                ++ret;
            }
            return ret;
        }

        template<typename Compare>
        void mergeSort(const_iterator before_first, const_iterator last,
            difference_type length, Compare comp)
        {
            // sort (before_first, last)
            // length is the number of elements in (before_first, last)

            if (length <= 1)
            {
                return;
            }

            auto halfLength = length / 2;

            // sort (before_first, middle)
            auto middle = std::next(before_first, halfLength + 1);
            mergeSort(before_first, middle, halfLength, comp);

            // sort (before_middle, last)
            auto before_middle = std::next(before_first, halfLength);
            mergeSort(before_middle, last, length - halfLength, comp);

            // merge (before_first, middle) and (before_middle, last)
            auto first = std::next(before_first);
            middle = std::next(before_middle);
            for (/* empty */; first != middle && middle != last; ++before_first)
            {
                if (comp(*middle, *first))
                {
                    splice_after(before_first, *this, before_middle);
                    middle = std::next(before_middle);
                }
                else
                {
                    ++first;
                }
            }
            if (middle != last)
            {
                splice_after(before_first, *this, before_first, last);
            }
        }

        void spliceAfter(const_iterator before_pos, forward_list & rhs,
            const_iterator before_first, const_iterator last)
        {
            // splice elements in (before_first, last) just after before_pos
            // before_pos may NOT be in (before_first, last)

            // (before_first, last) is empty, do nothing
            if (before_first == last || std::next(before_first) == last)
            {
                return;
            }

            if (this != &rhs || before_pos != before_first)
            {
                auto beforeLast = before_first;
                auto next = before_first;
                while (++next != last)
                {
                    ++beforeLast;
                }
                beforeLast.myNode->next = before_pos.myNode->next;
                before_pos.myNode->next = before_first.myNode->next;
                before_first.myNode->next = last.myNode;
            }
        }
        
        template<typename ... Args>
        iterator insertAfterN(const_iterator pos, size_type count, Args && ... args)
        {
            NodePtr node = pos.myNode;
            NodePtr first = node;
            NodePtr last = node->next;
            try
            {
                for (size_type i = 0; i != count; ++i)
                {
                    NodePtr newNode = createNode(node->next, std::forward<Args>(args)...);
                    node->next = newNode;
                    node = newNode;
                }
            }
            catch (...)
            {
                for (NodePtr tmp = first->next; tmp != last; tmp = tmp->next)
                {
                    destrotyNode(tmp);
                }
                first->next = last;
                throw;
            }
            return iterator(node);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
            iterator insertAfterRange(const_iterator pos, InputIt first, InputIt last)
        {
            NodePtr node = pos.myNode;
            NodePtr start = node;
            NodePtr finish = node->next;
            try
            {
                for (auto iter = first; iter != last; ++iter)
                {
                    NodePtr newNode = createNode(node->next, *iter);
                    node->next = newNode;
                    node = newNode;
                }
            }
            catch (...)
            {
                auto next = start->next;
                for (NodePtr tmp = next; tmp != finish; tmp = next)
                {
                    next = tmp->next;
                    destrotyNode(tmp);
                }
                start->next = finish;
                throw;
                throw;
            }
            return iterator(node);
        }

    }; // template class forward_list

    // static definitions

    template<typename T, typename Allocator>
    typename forward_list<T, Allocator>::NodeAlloc
        forward_list<T, Allocator>::nodeAlloc;

    template<typename T, typename Allocator>
    Allocator forward_list<T, Allocator>::dataAlloc;

    // non-member functions

    template<typename T, typename Allocator>
    inline void swap(forward_list<T, Allocator>& lhs, forward_list<T, Allocator>& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator==(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }

    template<typename T, typename Allocator>
    inline bool operator!=(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator<(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
            rhs.cbegin(), rhs.cend());
    }

    template< typename T, typename Allocator >
    inline bool operator<=(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return !(rhs < lhs);
    }

    template< typename T, typename Allocator >
    inline bool operator>(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return !(lhs <= rhs);
    }

    template< typename T, typename Allocator >
    bool operator>=(const forward_list<T, Allocator>& lhs,
        const forward_list<T, Allocator>& rhs)
    {
        return !(lhs < rhs);
    }
}