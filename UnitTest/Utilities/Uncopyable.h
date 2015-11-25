#pragma once

class Uncopyable
{
public:
    Uncopyable(int id = 0) :
        myId(id) { }

    Uncopyable(Uncopyable && rhs) = default;

    Uncopyable & operator=(Uncopyable && rhs) = default;

    Uncopyable(const Uncopyable &) = delete;

    Uncopyable & operator=(const Uncopyable &) = delete;

    bool operator==(const Uncopyable & rhs) const noexcept
    {
        return myId == rhs.myId;
    }

    bool operator!=(const Uncopyable & rhs) const noexcept
    {
        return !(*this == rhs);
    }

    bool operator<(const Uncopyable & rhs) const noexcept
    {
        return myId < rhs.myId;
    }

    bool operator>=(const Uncopyable & rhs) const noexcept
    {
        return !(*this < rhs);
    }

    bool operator>(const Uncopyable & rhs) const noexcept
    {
        return !(rhs >= *this);
    }

    bool operator<=(const Uncopyable & rhs) const noexcept
    {
        return !(*this > rhs);
    }

private:
    int myId = 0;
};
