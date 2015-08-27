#ifndef LAI_VECTOR_H
#define LAI_VECTOR_H

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace lai
{
    template<typename T, typename Allocator = std::allocator<T>>
    class vector
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using const_reference = const T &;
        using iterator = T *;
        using const_iterator = const T *;

    public:

        // constructors

        constexpr vector() noexcept = default;

        explicit vector(size_type count)
        {
            valueInitConstructN(count);
        }

        vector(size_type count, const T & value)
        {
            copyConstructN(count, value);
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
            vector(InputIt sourceFirst, InputIt sourceLast)
        {
            copyConstruct(sourceFirst, sourceLast);
        }

        vector(std::initializer_list<T> iList)
        {
            copyConstruct(iList.begin(), iList.end());
        }

        vector(const vector & rhs)
        {
            copyConstruct(rhs.cbegin(), rhs.cend());
        }

        vector(vector && rhs) noexcept :
            dataBegin(std::move(rhs.dataBegin)),
            dataEnd(std::move(rhs.dataEnd)),
            storageEnd(std::move(rhs.storageEnd))
        {
            rhs.dataBegin = nullptr;
            rhs.dataEnd = nullptr;
            rhs.storageEnd = nullptr;
        }

        vector & operator=(vector rhs)
        {
            swap(rhs);
            return *this;
        }

        void assign(size_type count, const T & value)
        {
            swap(vector(count, value));
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        void assign(InputIt first, InputIt last)
        {
            swap(vector(first, last));
        }

        void assign(std::initializer_list<T> iList)
        {
            swap(vector(iList));
        }

        ~vector()
        {
            deallocateAll();
        }

        // capacity operations

        void clear()
        {
            destroyRange(dataBegin, dataEnd);
            dataEnd = dataBegin;
        }

        bool empty() const noexcept
        {
            return dataBegin == dataEnd;
        }

        size_type size() const noexcept
        {
            return dataEnd - dataBegin;
        }

        size_type capacity() const noexcept
        {
            return storageEnd - dataBegin;
        }

        void reserve(size_type newCapacity)
        {
            if (newCapacity > capacity())
            {
                reallocate(newCapacity);
            }
        }

        void shrink_to_fit()
        {
            if (capacity() > size())
            {
                reallocate(size());
            }
        }

        // element access

        reference at(size_type pos)
        {
            checkRange(pos);
            return *(begin() + pos);
        }

        const_reference at(size_type pos) const
        {
            checkRange(pos);
            return *(begin() + pos);
        }

        reference operator[](size_type pos)
        {
            return *(begin() + pos);
        }

        const_reference operator[](size_type pos) const
        {
            return *(cbegin() + pos);
        }

        // iterator operations

        iterator begin() noexcept
        {
            return dataBegin;
        }

        const_iterator cbegin() const noexcept
        {
            return dataBegin;
        }

        iterator end() noexcept
        {
            return dataEnd;
        }

        const_iterator cend() const noexcept
        {
            return dataEnd;
        }

        reference front()
        {
            return *(begin());
        }

        const_reference front() const
        {
            return *(begin());
        }

        reference back()
        {
            return *(end() - 1);
        }

        const_reference back() const
        {
            return *(end() - 1);
        }

        // modifiers

        void swap(vector & rhs)
        {
            using std::swap;
            swap(dataBegin, rhs.dataBegin);
            swap(dataEnd, rhs.dataEnd);
            swap(storageEnd, rhs.storageEnd);
        }

        void resize(size_type count)
        {
            if (size() > count)
            {
                popBackN(size() - count);
            }
            else if (size() < count)
            {
                reserve(count);
                for (auto i = size(); i != count; ++i)
                {
                    valueInitConstructAt(dataEnd++);
                }
            }
        }

        void resize(size_type count, const T & value)
        {
            if (size() > count)
            {
                popBackN(size() - count);
            }
            else if (size() < count)
            {
                reserve(count);
                dataEnd = std::uninitialized_fill_n(dataEnd, count - size(), value);
            }
        }

        void push_back(const T & value)
        {
            if (isInside(std::addressof(value)))
            {
                size_type pos = std::addressof(value) - begin();
                checkReallocate();
                copyConstructAt(dataEnd++, *(dataBegin + pos));
            }
            else
            {
                checkReallocate();
                copyConstructAt(dataEnd++, value);
            }
        }

        void push_back(T && value)
        {
            if (isInside(std::addressof(value)))
            {
                size_type pos = std::addressof(value) - begin();
                checkReallocate();
                alloc.construct(dataEnd++, std::move(*(dataBegin + pos)));
            }
            else
            {
                checkReallocate();
                alloc.construct(dataEnd++, std::move(value));
            }
        }

        template<typename ... Args>
        void emplace_back(Args && ... args)
        {
            checkReallocate();
            alloc.construct(dataEnd++, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        iterator emplace(const_iterator pos, Args && ... args)
        {
            auto offset = pos - begin();
            emplace_back(std::forward<Args>(args)...);
            std::rotate(begin() + offset, end() - 1, end());
            return begin() + offset;
        }

        iterator insert(const_iterator pos, size_type count, const T & value)
        {
            auto offset = pos - begin();
            if (unusedCapacity() < count)
            {
                auto ptr = const_cast<iterator>(pos);
                auto newCapacity = growTo(size() + count);
                auto newDataBegin = allocateN(newCapacity);
                auto newDataEnd = std::uninitialized_copy(begin(), ptr, newDataBegin);
                newDataEnd = std::uninitialized_fill_n(newDataEnd, count, value);
                if (pos != end())
                {
                    newDataEnd = std::uninitialized_copy(ptr, end(), newDataEnd);
                }
                deallocateAll();
                dataBegin = newDataBegin;
                dataEnd = newDataEnd;
                storageEnd = dataBegin + newCapacity;

            }
            else
            {
                auto oldSize = size();
                std::uninitialized_fill_n(end(), count, value);
                dataEnd += count;
                std::rotate(begin() + offset, begin() + oldSize, end());
            }
            return begin() + offset;
        }

        iterator insert(const_iterator pos, const T & value)
        {
            return insert(pos, 1, value);
        }

        iterator insert(const_iterator pos, T && value)
        {
            return emplace(pos, std::move(value));
        }

        template<typename InputIt,
            typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        iterator insert(const_iterator pos, InputIt first, InputIt last)
        {
            auto offset = pos - begin();
            size_type count = last - first;
            if (unusedCapacity() < count)
            {
                auto ptr = const_cast<iterator>(pos);
                auto newCapacity = growTo(size() + count);
                auto newDataBegin = allocateN(newCapacity);
                auto newDataEnd = std::uninitialized_copy(begin(), ptr, newDataBegin);
                newDataEnd = std::uninitialized_copy(first, last, newDataEnd);
                if (pos != end())
                {
                    newDataEnd = std::uninitialized_copy(ptr, end(), newDataEnd);
                }
                deallocateAll();
                dataBegin = newDataBegin;
                dataEnd = newDataEnd;
                storageEnd = dataBegin + newCapacity;

            }
            else
            {
                auto oldSize = size();
                dataEnd = std::uninitialized_copy(first, last, dataEnd);
                std::rotate(begin() + offset, begin() + oldSize, end());
            }
            return begin() + offset;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> iList)
        {
            return insert(pos, iList.begin(), iList.end());
        }

        void pop_back()
        {
            alloc.destroy(--dataEnd);
        }

        iterator erase(const_iterator pos)
        {
            auto ptr = const_cast<iterator>(pos);
            moveRange(ptr + 1, dataEnd, ptr);
            alloc.destroy(--dataEnd);
            return ptr;
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            auto firstPtr = const_cast<iterator>(first);
            auto lastPtr = const_cast<iterator>(last);
            if (first == begin() && last == end())
            {
                clear();
            }
            else if (first != last)
            {
                auto length = last - first;
                moveRange(lastPtr, dataEnd, firstPtr);
                destroyRange(dataEnd - length, dataEnd);
                dataEnd -= length;
            }
            return firstPtr;
        }


    private:
        T * dataBegin = nullptr;
        T * dataEnd = nullptr;
        T * storageEnd = nullptr;
        static Allocator alloc;

    private:
        size_type unusedCapacity() const noexcept;
        void checkRange(size_type pos) const;
        bool isInside(const value_type * iter) const noexcept;
        T * allocateN(size_type capacity) const;
        template<typename InputIt, typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
        void copyConstruct(InputIt sourceFirst, InputIt sourceLast);
        void copyConstructAt(const value_type * pos, const value_type & val);
        void copyConstructN(size_type count, const value_type & val);
        void valueInitConstructAt(const value_type * pos);
        void valueInitConstructN(size_type count);
        void reallocate(size_type newCapacity);
        size_type growTo(size_type size);
        void checkReallocate();
        void moveRange(value_type * sourceFirst, value_type * sourceLast, value_type * destFirst);
        void moveBackward(value_type * sourceFirst, value_type * sourceLast, value_type * destLast);
        void destroyRange(value_type * first, value_type * last);
        void deallocateAll();
        void popBackN(size_type count);
    };

// non-member functions

    template<typename T, typename Allocator>
    inline void swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator==(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return (lhs.size() == rhs.size()) &&
            (std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()));
    }

    template<typename T, typename Allocator>
    inline bool operator!=(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Allocator>
    inline bool operator<(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
            rhs.cbegin(), rhs.cend());
    }

    template< typename T, typename Allocator >
    inline bool operator<=(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return !(rhs < lhs);
    }

    template< typename T, typename Allocator >
    inline bool operator>(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return !(lhs <= rhs);
    }

    template< typename T, typename Allocator >
    bool operator>=(const vector<T, Allocator>& lhs,
        const vector<T, Allocator>& rhs)
    {
        return !(lhs < rhs);
    }


// private vector utilities


    template<typename T, typename Allocator>
    Allocator vector<T, Allocator>::alloc;


    template<typename T, typename Allocator>
    inline typename vector<T, Allocator>::size_type
        vector<T, Allocator>::unusedCapacity() const noexcept
    {
        return static_cast<size_type>(storageEnd - dataEnd);
    }


    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::checkRange(size_type pos) const
    {
        if (pos >= size())
        {
            throw std::out_of_range("invalid lai::vector<T> index");
        }
    }


    template<typename T, typename Allocator>
    inline bool vector<T, Allocator>::isInside(const value_type * iter) const noexcept
    {
        return dataBegin <= iter && iter < dataEnd;
    }


    template<typename T, typename Allocator>
    inline T * vector<T, Allocator>::allocateN(size_type capacity) const
    {
        return alloc.allocate(capacity);
    }


    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::copyConstructAt(const value_type * pos, const value_type & val)
    {
        alloc.construct(pos, val);
    }


    template<typename T, typename Allocator>
    void vector<T, Allocator>::copyConstructN(size_type count, const value_type & val)
    {
        dataBegin = allocateN(count);
        dataEnd = std::uninitialized_fill_n(dataBegin, count, val);
        storageEnd = dataBegin + count;
    }

    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::valueInitConstructAt(const value_type * pos)
    {
        alloc.construct(pos, value_type());
    }

    template<typename T, typename Allocator>
    void vector<T, Allocator>::valueInitConstructN(size_type count)
    {
        dataBegin = allocateN(count);
        dataEnd = dataBegin;
        for (size_type i = 0; i != count; ++i)
        {
            valueInitConstructAt(dataEnd++);
        }
        storageEnd = dataBegin + count;
    }


    template<typename T, typename Allocator>
    template<typename InputIt, typename>
    void vector<T, Allocator>::copyConstruct(InputIt sourceFirst, InputIt sourceLast)
    {
        auto length = sourceLast - sourceFirst;
        dataBegin = allocateN(length);
        dataEnd = std::uninitialized_copy(sourceFirst, sourceLast, dataBegin);
        storageEnd = dataBegin + length;
    }


    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::checkReallocate()
    {
        if (unusedCapacity() == 0)
        {
            size_type newCapacity = size() ? 2 * size() : 1;
            reallocate(newCapacity);
        }
    }

    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::moveRange(value_type * sourceFirst, value_type * sourceLast, value_type * destFirst)
    {
        while (sourceFirst != sourceLast)
        {
            *destFirst++ = std::move(*sourceFirst++);
        }
    }

    template<typename T, typename Allocator>
    void vector<T, Allocator>::moveBackward(value_type * sourceFirst, value_type * sourceLast, value_type * destLast)
    {
        while (sourceLast != sourceFirst)
        {
            *--destLast = std::move(*--sourceLast);
        }
    }


    template<typename T, typename Allocator>
    void vector<T, Allocator>::reallocate(size_type newCapacity)
    {
        auto newDataBegin = allocateN(newCapacity);
        auto newDataEnd = newDataBegin;
        for (auto iter = dataBegin; iter != dataEnd; ++iter)
        {
            alloc.construct(newDataEnd++, std::move_if_noexcept(*iter));
        }
        deallocateAll();
        dataBegin = newDataBegin;
        dataEnd = newDataEnd;
        storageEnd = newDataBegin + newCapacity;
    }

    template<typename T, typename Allocator>
    inline typename vector<T, Allocator>::size_type
        vector<T, Allocator>::growTo(size_type size)
    {
        auto newCapacity = capacity() * 2;
        if (newCapacity < size)
        {
            newCapacity = size;
        }
        return newCapacity;
    }


    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::destroyRange(value_type * first, value_type * last)
    {
        while (last != first)
        {
            alloc.destroy(--last);
        }
    }


    template<typename T, typename Allocator>
    void vector<T, Allocator>::deallocateAll()
    {
        if (dataBegin)
        {
            destroyRange(dataBegin, dataEnd);
            alloc.deallocate(dataBegin, capacity());
            dataBegin = nullptr;
            dataEnd = nullptr;
            storageEnd = nullptr;
        }
    }


    template<typename T, typename Allocator>
    inline void vector<T, Allocator>::popBackN(size_type count)
    {
        while (count-- > 0)
        {
            pop_back();
        }
    }
};

#endif // !LAI_VECTOR_H

