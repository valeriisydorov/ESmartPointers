#pragma once


#include <cstddef>
#include <cassert>
#include "DefaultDelete.h"
#include "../EUniquePtr/EUniquePtr.h"


template <typename T, typename Deleter = DefaultDelete<T>>
class ControlBlock
{
public:
    using ObjectType = T;
    using ObjectPointerType = ObjectType*;
    using DeleterType = Deleter;
    using CountType = std::size_t;
    using ObjectUniquePointerType = EUniquePtr<ObjectType, DeleterType>;

    ControlBlock() = delete;
    explicit ControlBlock(ObjectUniquePointerType obj);
    ControlBlock(const ControlBlock&) = delete;
    ControlBlock(ControlBlock&& other) noexcept = delete;
    ControlBlock& operator=(const ControlBlock&) = delete;
    ControlBlock& operator=(ControlBlock&& rhs) noexcept = delete;
    ~ControlBlock();

    ObjectPointerType getObject() const;
    CountType getSharedCount() const;
    CountType getWeakCount() const;

    void incrementShared();
    void decrementShared();
    void incrementWeak();
    void decrementWeak();

    void releaseShared();
    void releaseWeak();

private:
    ObjectUniquePointerType object;
    CountType sharedCount;
    CountType weakCount;

};


template <typename T, typename Deleter>
ControlBlock<T, Deleter>::ControlBlock(ObjectUniquePointerType obj)
    : object(std::move(obj))
    , sharedCount(1)
    , weakCount(0)
{
    assert(object != nullptr && "ControlBlock: Object should not be null");
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
    return object.operator->();
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
        object.reset(nullptr);
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








