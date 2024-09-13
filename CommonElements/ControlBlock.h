#pragma once

#include <cstddef>
#include <cassert>
#include "DefaultDelete.h"


template <typename T, typename Deleter = DefaultDelete<T>>
class ControlBlock
{
public:
    using ObjectType = T;
    using ObjectPointerType = ObjectType*;
    using DeleterType = Deleter;
    using CountType = std::size_t;

    ControlBlock() = delete;
    explicit ControlBlock(ObjectPointerType obj = nullptr, DeleterType del = Deleter(), CountType shared = 1);
    ControlBlock(const ControlBlock&) = delete;
    ControlBlock(ControlBlock&& other) noexcept;
    ControlBlock& operator=(const ControlBlock&) = delete;
    ControlBlock& operator=(ControlBlock&& rhs) noexcept;
    ~ControlBlock();

    ObjectPointerType getObject() const;
    DeleterType getDeleter() const;
    CountType getSharedCount() const;
    CountType getWeakCount() const;

    void incrementShared();
    void decrementShared();
    void incrementWeak();
    void decrementWeak();

    void releaseShared();
    void releaseWeak();

private:
    ObjectPointerType object;
    DeleterType deleter;
    CountType sharedCount;
    CountType weakCount;

};


template <typename T, typename Deleter>
ControlBlock<T, Deleter>::ControlBlock(ObjectPointerType obj, DeleterType del, CountType shared)
    : object(obj)
    , deleter(del)
    , sharedCount(shared)
    , weakCount(0)
{
    assert(sharedCount >= 0);
}

template <typename T, typename Deleter>
ControlBlock<T, Deleter>::ControlBlock(ControlBlock&& other) noexcept
    : object(other.object)
    , deleter(other.deleter)
    , sharedCount(other.sharedCount)
    , weakCount(other.weakCount)
{
    other.object = nullptr;
    other.sharedCount = 0;
    other.weakCount = 0;
}

template <typename T, typename Deleter>
ControlBlock<T, Deleter>& ControlBlock<T, Deleter>::operator=(ControlBlock&& rhs) noexcept
{
    if (this != &rhs)
    {
        if (object != nullptr)
        {
            deleter(object);
        }

        object = rhs.object;
        deleter = rhs.deleter;
        sharedCount = rhs.sharedCount;
        weakCount = rhs.weakCount;

        rhs.object = nullptr;
        rhs.sharedCount = 0;
        rhs.weakCount = 0;
    }

    return *this;
}

template <typename T, typename Deleter>
ControlBlock<T, Deleter>::~ControlBlock()
{
    if (sharedCount > 0)
    {
        releaseShared();
    }
    if (weakCount > 0)
    {
        releaseWeak();
    }
}

template <typename T, typename Deleter>
typename ControlBlock<T, Deleter>::ObjectPointerType ControlBlock<T, Deleter>::getObject() const
{
    return object;
}

template <typename T, typename Deleter>
typename ControlBlock<T, Deleter>::DeleterType ControlBlock<T, Deleter>::getDeleter() const
{
    return deleter;
}

template <typename T, typename Deleter>
typename ControlBlock<T, Deleter>::CountType ControlBlock<T, Deleter>::getSharedCount() const
{
    return sharedCount;
}

template <typename T, typename Deleter>
typename ControlBlock<T, Deleter>::CountType ControlBlock<T, Deleter>::getWeakCount() const
{
    return weakCount;
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::incrementShared()
{
    ++sharedCount;
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::decrementShared()
{
    if (sharedCount > 0)
    {
        --sharedCount;
    }
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::incrementWeak()
{
    ++weakCount;
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::decrementWeak()
{
    if (weakCount > 0)
    {
        --weakCount;
    }
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::releaseShared()
{
    if (--sharedCount == 0)
    {
        if (object != nullptr)
        {
            deleter(object);
            object = nullptr;
        }
        releaseWeak();
    }
}

template <typename T, typename Deleter>
void ControlBlock<T, Deleter>::releaseWeak()
{
    if (sharedCount == 0 && --weakCount == 0)
    {
        delete this;
    }
}








