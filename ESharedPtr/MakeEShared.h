#pragma once


#include <cstddef>
#include <utility>
#include "ESharedPtr.h"
#include "../EUniquePtr/EUniquePtr.h"


template<typename T, typename... Args>
ESharedPtr<T> makeEShared(Args&&... args)
{
    EUniquePtr<T, DefaultDelete<T>> uniquePtr(new T(std::forward<Args>(args)...));

    return ESharedPtr<T>(new ControlBlock<T>(std::move(uniquePtr)));
}

template<typename T, typename Deleter, typename... Args, typename = std::enable_if_t<std::is_invocable_v<Deleter, T*>>>
ESharedPtr<T> makeEShared(Deleter del, Args&&... args)
{
    EUniquePtr<T, Deleter> uniquePtr(new T(std::forward<Args>(args)...), del);

    return ESharedPtr<T>(new ControlBlock<T>(std::move(uniquePtr)));
}

template <typename T>
ESharedPtr<T[]> makeEShared(std::size_t size)
{
    EUniquePtr<T[], DefaultDelete<T[]>> uniquePtr(new T[size]);

    return ESharedPtr<T[]>(new ControlBlock<T[]>(std::move(uniquePtr)));
}

template <typename T, typename Deleter, typename = std::enable_if_t<std::is_invocable_v<Deleter, T[]>>>
ESharedPtr<T[]> makeEShared(std::size_t size, Deleter del)
{
    EUniquePtr<T[], Deleter> uniquePtr(new T[size], del);

    return ESharedPtr<T[]>(new ControlBlock<T[]>(std::move(uniquePtr)));
}
