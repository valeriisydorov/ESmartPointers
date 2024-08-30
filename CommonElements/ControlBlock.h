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
    if (obj != nullptr)
    {
        assert(sharedCount > 0);
    }
    else
    {
        assert(sharedCount == 0);
    }
}