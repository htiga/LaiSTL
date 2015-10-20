#pragma once
#include <memory>
#include <iterator>
#include <utility>
#include <type_traits>
#include <initializer_list>

namespace lai
{
    template<typename TList, typename TReference, typename TPointer, typename NodePtr>
    class ListIterator
    {
        friend TList;
    public:
        using pointer           = TPointer;
        using reference         = TReference;
        using value_type        = typename TList::value_type;
        using size_type         = typename TList::size_type;
        using difference_type   = typename TList::difference_type;
        using iterator_category = std::bidirectional_iterator_tag;

        constexpr ListIterator() noexcept = default;

        ListIterator(NodePtr node) :
            node(node) { }

        // Convertion from iterator to const_iterator.
        operator ListIterator<TList,
            typename TList::const_reference,
            typename TList::const_pointer,
            NodePtr>() const
        {
            return ListIterator<TList,
                typename TList::const_reference,
                typename TList::const_pointer,
                NodePtr>(node);
        }

        ListIterator & operator++()
        {
            node = node->next;
            return *this;
        }

        ListIterator operator++(int)
        {
            ListIterator old = *this;
            ++*this;
            return old;
        }

        ListIterator & operator--()
        {
            node = node->prev;
            return *this;
        }

        ListIterator operator--(int)
        {
            ListIterator old = *this;
            ++*this;
            return old;
        }

        reference operator*() const
        {
            return node->value;
        }

        pointer operator->() const
        {
            return std::addressof(operator*());
        }

        // Friend functions defined inside class to support operations between iterator and const_iterator.

        friend bool operator==(const ListIterator & lhs, const ListIterator & rhs)
        {
            return lhs.node == rhs.node;
        }

        friend bool operator!=(const ListIterator & lhs, const ListIterator & rhs)
        {
            return !(lhs == rhs);
        }

    private:
        NodePtr node = nullptr;
    }; // template class ListIterator


    template<typename T, typename Allocator = std::allocator<T>>
    class list
    {
    private:
        struct ListNode;
        using NodeAllocator = typename Allocator::template rebind<ListNode>::other;
        using DataAllocator = Allocator;
        using NodePtr       = typename NodeAllocator::pointer;
        static NodeAllocator nodeAlloc;
        static DataAllocator dataAlloc;
    public:
        using value_type             = T;
        using allocator_type         = Allocator;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using reference              = value_type &;
        using const_reference        = const value_type &;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator               = ListIterator<list, reference, pointer, NodePtr>;
        using const_iterator         = ListIterator<list, const_reference, const_pointer, NodePtr>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    public:
        list() :
            myHead(createHead()) { }

        explicit list(size_type count) :
            myHead(createHead())
        {
            insertN(end(), count);
        }

        list(size_type count, const value_type & val) :
            myHead(createHead())
        {
            insertN(end(), count, val);
        }

        template<typename InputIt,
            typename = std::enable_if_t <!std::is_integral<InputIt>::value>>
        list(InputIt first, InputIt last) :
            myHead(createHead())
        {
            insertRange(end(), first, last);
        }

        list(std::initializer_list<value_type> init) :
            myHead(createHead())
        {
            insertRange(end(), init.begin(), init.end());
        }

        list(const list & rhs) :
            myHead(createHead())
        {
            insertRange(end(), rhs.begin(), rhs.end());
        }

        list(list && rhs) :
            myHead(std::move(rhs.myHead)),
            mySize(std::move(rhs.mySize))
        {
            rhs.myHead = createHead();
            rhs.mySize = 0;
        }

        ~list()
        {
            if (myHead)
            {
                clear();
                deallocateHead(myHead);
                myHead = nullptr;
            }
        }

        list & operator=(const list & rhs)
        {
            list copied(rhs);
            swap(copied);
            return *this;
        }

        list & operator=(list && rhs)
        {
            list moved(std::move(rhs));
            swap(moved);
            return *this;
        }

        list & operator=(std::initializer_list<value_type> iList)
        {
            list newList(iList);
            swap(newList);
            return *this;
        }

        void assign(size_type count, const value_type & val)
        {
            list newList(count, val);
            swap(newList);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
            void assign(InputIt first, InputIt last)
        {
            list newList(first, last);
            swap(newList);
        }

        void assign(std::initializer_list<value_type> iList)
        {
            list newList(iList);
            swap(newList);
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return *--end();
        }

        const_reference back() const
        {
            return *--end();
        }

        iterator begin()
        {
            return iterator(myHead->next);
        }

        const_iterator begin() const
        {
            return const_iterator(myHead->next);
        }

        const_iterator cbegin() const
        {
            return begin();
        }

        iterator end()
        {
            return iterator(myHead);
        }

        const_iterator end() const
        {
            return const_iterator(myHead);
        }

        const_iterator cend() const
        {
            return end();
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const
        {
            return rbegin();
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const
        {
            return rend();
        }

        bool empty() const noexcept
        {
            return size() == 0;
        }

        size_type size() const noexcept
        {
            return mySize;
        }

        void clear()
        {
            auto curr = myHead->next;
            while (curr != myHead)
            {
                auto next = curr->next;
                deallocateNode(curr);
                curr = next;
            }
            mySize = 0;
            myHead->prev = myHead;
            myHead->next = myHead;
        }

        iterator insert(const_iterator pos, const value_type & val)
        {
            return emplace(pos, val);
        }

        iterator insert(const_iterator pos, value_type && val)
        {
            return emplace(pos, std::move(val));
        }

        iterator insert(const_iterator pos, size_type count, const value_type & val)
        {
            return insertN(pos, count, val);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
            iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            return insertRange(pos, first, last);
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> iList)
        {
            return insertRange(pos, iList.begin(), iList.end());
        }

        template<typename ... Args>
        iterator emplace(const_iterator pos, Args && ... args)
        {
            auto node = pos.node;
            auto newNode = createNode(node->prev, node, std::forward<Args>(args)...);
            node->prev->next = newNode;
            node->prev = newNode;
            mySize += 1;
            return iterator(newNode);
        }

        iterator erase(const_iterator pos)
        {
            auto node = pos.node;
            auto ret = node->next;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            deallocateNode(node);
            mySize -= 1;
            return iterator(ret);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            auto firstNode = first.node;
            auto lastNode = last.node;
            mySize -= std::distance(first, last);
            firstNode->prev->next = lastNode;
            lastNode->prev = firstNode->prev;
            while (firstNode != lastNode)
            {
                auto next = firstNode->next;
                deallocateNode(firstNode);
                firstNode = next;
            }
            return iterator(lastNode);
        }

        void push_back(const value_type & val)
        {
            insert(end(), val);
        }

        void push_back(value_type && val)
        {
            insert(end(), std::move(val));
        }

        template<typename ... Args>
        void emplace_back(Args && ... args)
        {
            emplace(end(), std::forward<Args>(args)...);
        }

        void pop_back()
        {
            erase(--end());
        }

        void push_front(const value_type & val)
        {
            insert(begin(), val);
        }

        void push_front(value_type && val)
        {
            insert(begin(), std::move(val));
        }

        template<typename ... Args>
        void emplace_front(Args && ... args)
        {
            emplace(begin(), std::forward<Args>(args)...);
        }

        void pop_front()
        {
            erase(begin());
        }

        void resize(size_type newSize)
        {
            if (newSize < size())
            {
                while (newSize < size())
                {
                    pop_back();
                }
            }
            else
            {
                insertN(end(), newSize - size());
            }
        }

        void resize(size_type newSize, const value_type & val)
        {
            if (newSize < size())
            {
                while (newSize < size())
                {
                    pop_back();
                }
            }
            else
            {
                insertN(end(), newSize - size(), val);
            }
        }

        void swap(list & rhs) noexcept
        {
            using std::swap;
            swap(myHead, rhs.myHead);
            swap(mySize, rhs.mySize);
        }

        void merge(list & rhs)
        {
            merge(rhs, std::less<>());
        }

        template<typename Compare>
        void merge(list & rhs, Compare comp)
        {
            mySize += rhs.mySize;
            rhs.mySize = 0;
            auto other = rhs.begin().node;
            auto self = begin().node;
            while (other != rhs.myHead && self != myHead)
            {
                if (comp(other->value, self->value))
                {
                    auto next = other->next;
                    transfer(self, other, other->next);
                    other = next;
                }
                else
                {
                    self = self->next;
                }
            }
            if (other != rhs.myHead)
            {
                transfer(self, other, rhs.myHead);
            }
        }

        void splice(const_iterator pos, list & rhs)
        {
            splice(pos, rhs, rhs.begin(), rhs.end());
        }

        void splice(const_iterator pos, list & rhs, const_iterator it)
        {
            mySize += 1;
            rhs.mySize -= 1;
            transfer(pos.node, it.node, it.node->next);
        }

        void splice(const_iterator pos, list & rhs, const_iterator first, const_iterator last)
        {
            if (std::addressof(rhs) != this || last != pos)
            {
                auto length = std::distance(first, last);
                mySize += length;
                rhs.mySize -= length;
                transfer(pos.node, first.node, last.node);
            }
        }

        void remove(const value_type & val)
        {
            remove_if([&val](const value_type & value) { return value == val; });
        }

        template<typename UnaryPredicate>
        void remove_if(UnaryPredicate pred)
        {
            for (auto iter = begin(); iter != end();)
            {
                if (pred(*iter))
                {
                    iter = erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }

        template<typename BinaryPredicate>
        void unique(BinaryPredicate pred)
        {
            auto mark = begin();
            auto next = ++begin();
            while (next != end())
            {
                if (pred(*mark, *next))
                {
                    next = erase(next);
                }
                else
                {
                    mark = next;
                    ++next;
                }
            }
        }

        void reverse()
        {
            using std::swap;
            auto node = myHead->next;
            while (node != myHead)
            {
                auto next = node->next;
                swap(node->prev, node->next);
                node = next;
            }
            swap(myHead->prev, myHead->next);
        }

        void unique()
        {
            unique(std::equal_to<>());
        }

        void sort()
        {
            sort(std::less<>());
        }

        template<typename Compare>
        void sort(Compare comp)
        {
            mergeSort(begin(), end(), size(), comp);
        }

    private:

        NodePtr allocateNode(size_type count)
        {
            return nodeAlloc.allocate(count);
        }

        void deallocateNode(NodePtr node)
        {
            node->prev = nullptr;
            node->next = nullptr;
            dataAlloc.destroy(std::addressof(node->value));
            nodeAlloc.deallocate(node, 1);
        }

        void deallocateHead(NodePtr head)
        {
            head->prev = nullptr;
            head->next = nullptr;
            nodeAlloc.deallocate(head, 1);
        }

        NodePtr createHead()
        {
            NodePtr node = allocateNode(1);
            node->prev = node;
            node->next = node;
            return node;
        }

        template<typename ... Args>
        NodePtr createNode(NodePtr prev, NodePtr next, Args && ... args)
        {
            NodePtr node = allocateNode(1);
            node->prev = prev;
            node->next = next;
            try
            {
                dataAlloc.construct(std::addressof(node->value), std::forward<Args>(args)...);
            }
            catch (...)
            {
                deallocateNode(node);
                throw;
            }
            return node;
        }

        template<typename ... Args>
        iterator insertN(const_iterator pos, size_type count, Args && ... args)
        {
            auto node = pos.node;
            auto start = node->prev;
            auto temp = start;
            try
            {
                for (int i = 0; i != count; ++i)
                {
                    auto newNode = createNode(temp, node, std::forward<Args>(args)...);
                    temp->next = newNode;
                    node->prev = newNode;
                    temp = newNode;
                }
            }
            catch (...)
            {
                node->prev = start;
                start->next = node;
                while (temp != start)
                {
                    auto prev = temp->prev;
                    deallocateNode(temp);
                    temp = prev;
                }
                throw;
            }
            mySize += count;
            return iterator(start->next);
        }

        template<typename InputIt,
            typename = std::enable_if_t <!std::is_integral<InputIt>::value >>
        iterator insertRange(const_iterator pos, InputIt first, InputIt last)
        {
            auto node = pos.node;
            auto start = node->prev;
            auto temp = start;
            size_type count = 0;
            try
            {
                for (auto iter = first; iter != last; ++iter, ++count)
                {
                    auto newNode = createNode(temp, node, *iter);
                    temp->next = newNode;
                    node->prev = newNode;
                    temp = newNode;
                }
            }
            catch (...)
            {
                node->prev = start;
                start->next = node;
                while (temp != start)
                {
                    auto prev = temp->prev;
                    deallocateNode(temp);
                    temp = prev;
                }
                throw;
            }
            mySize +=  count;
            return iterator(start->next);
        }

        // transfer all elements in [first, last) to pos front.
        void transfer(NodePtr pos, NodePtr first, NodePtr last)
        {
            if (first != last && pos != last)
            {
                last->prev->next = pos;
                first->prev->next = last;
                pos->prev->next = first;
                auto tmp = pos->prev;
                pos->prev = last->prev;
                last->prev = first->prev;
                first->prev = tmp;
            }
        }

        // sort [first, last)
        // return the first iterator of sorted list
        template<typename Compare>
        iterator mergeSort(iterator first, iterator last, size_type length, Compare comp)
        {
            if (length <= 1)
            {
                return first;
            }
            auto halfLen = length / 2;
            auto middle = std::next(first, halfLen);

            // sort [first, middle) and [middle, last)
            first = mergeSort(first, middle, halfLen, comp);
            middle = mergeSort(middle, last, length - halfLen, comp);
            auto newFirst = middle == last ? first :
                            first == middle ? middle :
                            comp(*middle, *first) ? middle : first;

            // merge [first, middle) and [middle, last)
            while (first != middle && middle != last)
            {
                if (comp(*middle, *first))
                {
                    auto next = middle.node->next;
                    transfer(first.node, middle.node, next);
                    middle.node = next;
                }
                else
                {
                    ++first;
                }
            }
            return newFirst;
        }

        NodePtr myHead = nullptr;
        size_type mySize = 0;

        struct ListNode
        {
            NodePtr prev = NodePtr();
            NodePtr next = NodePtr();
            value_type value;
        };

    }; // template class list

    // static definitions

    template<typename T, typename Allocator>
    typename list<T, Allocator>::NodeAllocator
        list<T, Allocator>::nodeAlloc;

    template<typename T, typename Allocator>
    typename list<T, Allocator>::DataAllocator
        list<T, Allocator>::dataAlloc;

    // non-member functions

    template<typename T, typename Allocator>
    inline void swap(list<T, Allocator>& lhs, list<T, Allocator>& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator==(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return (lhs.size() == rhs.size()) &&
            (std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()));
    }

    template<typename T, typename Allocator>
    inline bool operator!=(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator<(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
            rhs.cbegin(), rhs.cend());
    }

    template< typename T, typename Allocator >
    inline bool operator<=(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return !(rhs < lhs);
    }

    template< typename T, typename Allocator >
    inline bool operator>(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return !(lhs <= rhs);
    }

    template< typename T, typename Allocator >
    bool operator>=(const list<T, Allocator>& lhs,
        const list<T, Allocator>& rhs)
    {
        return !(lhs < rhs);
    }
}