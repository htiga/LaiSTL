#ifndef LAI_DEQUE_H
#define LAI_DEQUE_H

#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

namespace lai
{
    template<typename TDeque, typename TReference, typename TPointer>
    class DequeIterator
    {
    public:
        using pointer = TPointer;
        using reference = TReference;
        using value_type = typename TDeque::value_type;
        using difference_type = typename TDeque::difference_type;
        using iterator_category = std::random_access_iterator_tag;
        using size_type = typename TDeque::size_type;

        constexpr DequeIterator() noexcept = default;

        DequeIterator(const TDeque * dequePtr, size_type off) :
            deqPtr(dequePtr),
            offset(off) { }

        // Convertion from iterator to const_iterator.
        operator DequeIterator<TDeque,
                               typename TDeque::const_reference,
                               typename TDeque::const_pointer>() const
        {
            return DequeIterator<TDeque,
                                 typename TDeque::const_reference,
                                 typename TDeque::const_pointer>(deqPtr, offset);
        }

        DequeIterator & operator++()
        {
            ++offset;
            return *this;
        }

        DequeIterator operator++(int)
        {
            DequeIterator old = *this;
            ++*this;
            return old;
        }

        reference operator*() const
        {
            auto blockIndex = deqPtr->getBlockIndex(offset);
            auto elemIndex = deqPtr->getElemIndex(offset);
            return deqPtr->map[blockIndex][elemIndex];
        }

        pointer operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        DequeIterator & operator--()
        {
            --offset;
            return *this;
        }

        DequeIterator operator--(int)
        {
            DequeIterator old = *this;
            --*this;
            return old;
        }

        DequeIterator & operator+=(difference_type off)
        {
            offset += off;
            return *this;
        }

        DequeIterator operator+(difference_type off) const
        {
            auto old = *this;
            return (old += off);
        }

        DequeIterator & operator-=(difference_type off)
        {
            return (*this += -off);
        }

        DequeIterator operator-(difference_type off) const
        {
            auto old = *this;
            return (old -= off);
        }

        reference operator[](difference_type off) const
        {
            return *(*this + off);
        }

    private:
        const TDeque * deqPtr = nullptr;
        size_type offset = 0;

        // Friend functions defined inside class to support operations between iterator and const_iterator.

        friend difference_type operator-(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return static_cast<difference_type>(offset - rhs.offset)
        }

        friend bool operator==(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return lhs.offset == rhs.offset;
        }

        friend bool operator!=(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return !(lhs.offset == rhs.offset);
        }

        friend bool operator<(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return lhs.offset < rhs.offset;
        }

        friend bool operator >=(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return !(lhs < rhs);
        }

        friend bool operator>(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return (rhs < lhs);
        }

        friend bool operator<=(const DequeIterator & lhs, const DequeIterator & rhs)
        {
            return !(rhs < lhs);
        }

        // Support expression <n + iterator>.

        friend DequeIterator operator+(difference_type off, const DequeIterator & rhs)
        {
            auto old = *this;
            return (old += off);
        }
    };


    template<typename T, typename Allocator = std::allocator<T>>
    class deque
    {
    public:
        using allocator_type = Allocator;
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = DequeIterator<deque, reference, pointer>;
        using const_iterator = DequeIterator<deque, const_reference, const_pointer>;
        friend iterator;
        friend const_iterator;

    public:
        // constructors

        constexpr deque() = default;

        explicit deque(size_type count)
        {
            resize(count);
        }

        deque(size_type count, const T & val)
        {
            resize(count, val);
        }

        template<typename InputIt,
            typename = std::enable_if_t < !std::is_integral<InputIt>::value >>
        deque(InputIt first, InputIt last)
        {
            constructRange(first, last);
        }

        deque(std::initializer_list<T> il)
        {
            constructRange(il.begin(), il.end());
        }

        deque(const deque & rhs)
        {
            constructRange(rhs.cbegin(), rhs.cend());
        }

        // element access

        reference at(size_type pos)
        {
            if (size() <= pos)
            {
                throwOutOfRange();
            }
            return *(begin() + pos);
        }

        const_reference at(size_type pos) const
        {
            if (size() <= pos)
            {
                throwOutOfRange();
            }
            return *(cbegin() + pos);
        }

        reference operator[](size_type pos)
        {
            return *(begin() + pos);
        }

        const_reference operator[](size_type pos) const
        {
            return *(cbegin() + pos);
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *cbegin();
        }

        reference back()
        {
            return *(end() - 1);
        }

        const_reference back() const
        {
            return *(cend() - 1);
        }

        // modifiers

        void pop_front()
        {
            destroyAt(map[getBlockIndex(offset)] + getElemIndex(offset));
            ++offset;
            --dequeSize;
        }

        void pop_back()
        {
            auto off = offset + dequeSize - 1;
            destroyAt(map[getBlockIndex(off)] + getElemIndex(off));
            --dequeSize;
        }

        void push_front(const value_type & val)
        {
            size_type newOff = offset;
            constructAt(getPushFrontPos(newOff), val);
            ++dequeSize;
            offset = newOff;
        }

        void push_front(value_type && val)
        {
            size_type newOff = offset;
            constructAt(getPushFrontPos(newOff), std::move(val));
            ++dequeSize;
            offset = newOff;
        }

        void push_back(const value_type & val)
        {
            constructAt(getPushBackPos(), val);
            ++dequeSize;
        }

        void push_back(value_type && val)
        {
            constructAt(getPushBackPos(), std::move(val));
            ++dequeSize;
        }

        template<typename ... Args>
        void emplace_front(Args && ... args)
        {
            size_type newOff = offset;
            dataAlloc.construct(getPushFrontPos(newOff), std::forward<Args>(args)...);
            ++dequeSize;
            offset = newOff;
        }

        template<typename ... Args>
        void emplace_back(Args && ... args)
        {
            dataAlloc.construct(getPushBackPos(), std::forward<Args>(args)...);
            ++dequeSize;
        }

        // iterators

        iterator begin()
        {
            return iterator(this, offset);
        }

        const_iterator cbegin() const
        {
            return const_iterator(this, offset);
        }

        iterator end()
        {
            return iterator(this, offset + dequeSize);
        }

        const_iterator cend() const
        {
            return const_iterator(this, offset + dequeSize);
        }

        // Capacity operations

        size_type size() const noexcept
        {
            return dequeSize;
        }

        bool empty() const noexcept
        {
            return size() == 0;
        }

        void resize(size_type count)
        {
            while (size() > count)
            {
                pop_back();
            }
            while (size() < count)
            {
                emplace_back();
            }
        }

        void resize(size_type count, const value_type & val)
        {
            while (size() > count)
            {
                pop_back();
            }
            while (size() < count)
            {
                emplace_back(val);
            }
        }

    private:
        static constexpr size_type INIT_MAP_SIZE = 1;
        static constexpr size_type BLOCK_SIZE = sizeof(value_type) <= 1 ? 16
            : sizeof(value_type) <= 2 ? 8
            : sizeof(value_type) <= 4 ? 4
            : sizeof(value_type) <= 8 ? 2 : 1;
        using MapAllocator = typename Allocator::template rebind<value_type *>::other;
        using MapPtr = typename MapAllocator::pointer;
        static Allocator dataAlloc;
        static MapAllocator mapAlloc;

    private:
        MapPtr map = MapPtr();
        size_type mapSize = 0;
        size_type dequeSize = 0;
        size_type offset = 0;

    private:
        // private utilities

        value_type * allocateBlock()
        {
            return dataAlloc.allocate(BLOCK_SIZE);
        }

        MapPtr allocateMap(size_type size)
        {
            return mapAlloc.allocate(size);
        }

        void deallocateMap(MapPtr mapPtr, size_type mapCapacity)
        {
            mapAlloc.deallocate(mapPtr, mapCapacity);
        }

        void destroyAt(value_type * pos)
        {
            dataAlloc.destroy(pos);
        }

        void constructAt(value_type * pos, const value_type & val)
        {
            dataAlloc.construct(pos, val);
        }

        void constructAt(value_type * pos, value_type && val)
        {
            dataAlloc.construct(pos, std::move(val));
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        void constructRange(InputIt first, InputIt last)
        {
            while (first != last)
            {
                emplace_back(*first++);
            }
        }

        size_type getBlockIndex(size_type off) const noexcept
        {
            return (off / BLOCK_SIZE) % mapSize;
        }

        size_type getElemIndex(size_type off) const noexcept
        {
            return off % BLOCK_SIZE;
        }

        bool isMapFull() const noexcept
        {
            return mapSize <= (dequeSize + BLOCK_SIZE) / BLOCK_SIZE;
        }

        void throwOutOfRange() const
        {
            throw std::out_of_range("Invalid deque<T> subscript");
        }

        value_type * getPushBackPos()
        {
            size_type off = dequeSize + offset;
            if (off % BLOCK_SIZE == 0 && isMapFull())
            {
                growMap(1);
            }
            size_type blockIndex = getBlockIndex(off);
            if (!map[blockIndex])
            {
                map[blockIndex] = allocateBlock();
            }
            return map[blockIndex] + getElemIndex(off);
        }

        value_type * getPushFrontPos(size_type & newOff)
        {
            if (offset % BLOCK_SIZE == 0 && isMapFull())
            {
                growMap(1);
            }
            newOff = (offset ? offset : mapSize * BLOCK_SIZE) - 1;
            size_type blockIndex = getBlockIndex(newOff);
            if (!map[blockIndex])
            {
                map[blockIndex] = allocateBlock();
            }
            return map[blockIndex] + getElemIndex(newOff);
        }

        void growMap(size_type sizeNeeded)
        {
            size_type newSize = mapSize ? mapSize * 2 : INIT_MAP_SIZE;
            while (newSize - mapSize < sizeNeeded)
            {
                newSize *= 2;
            }
            auto newMap = allocateMap(newSize);
            auto firstBlockIndex = offset / BLOCK_SIZE;
            auto mPtr = newMap + firstBlockIndex;
            mPtr = std::uninitialized_copy(map + firstBlockIndex, map + mapSize, mPtr);
            mPtr = std::uninitialized_copy(map, map + firstBlockIndex, mPtr);
            std::uninitialized_fill(mPtr, newMap + newSize, pointer());
            std::uninitialized_fill(newMap, newMap + firstBlockIndex, pointer());
            deallocateMap(map, mapSize);
            map = newMap;
            mapSize = newSize;
        }
    };

    // static definitions

    template<typename T, typename Allocator = std::allocator<T>>
    typename deque<T, Allocator>::allocator_type
        deque<T, Allocator>::dataAlloc;


    template<typename T, typename Allocator = std::allocator<T>>
    typename deque<T, Allocator>::MapAllocator
        deque<T, Allocator>::mapAlloc;
}

#endif // !LAI_DEQUE_H
