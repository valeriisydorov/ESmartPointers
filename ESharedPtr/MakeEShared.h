#pragma once

#include <cstddef>
#include <utility>
#include "ESharedPtr.h"


template<typename T, typename... Args>
ESharedPtr<T> makeEShared(Args&&... args)
{
    T* object = new T(std::forward<Args>(args)...);
    ControlBlock<T>* controlBlock = new ControlBlock<T>(object, DefaultDelete<T>());

    return ESharedPtr<T>(controlBlock);
}

template<typename T, typename Deleter, typename... Args, typename = std::enable_if_t<std::is_invocable_v<Deleter, T*>>>
ESharedPtr<T> makeEShared(Deleter del, Args&&... args)
{
    T* object = new T(std::forward<Args>(args)...);
    ControlBlock<T>* controlBlock = new ControlBlock<T>(object, del);

    return ESharedPtr<T>(controlBlock);
}

template <typename T>
ESharedPtr<T[]> makeEShared(std::size_t size)
{
    T* array = new T[size];
    ControlBlock<T[]>* controlBlock = new ControlBlock<T[]>(array, DefaultDelete<T[]>());

    return ESharedPtr<T[]>(controlBlock);
}

template <typename T, typename Deleter, typename = std::enable_if_t<std::is_invocable_v<Deleter, T*>>>
ESharedPtr<T[]> makeEShared(std::size_t size, Deleter del)
{
    T* array = new T[size];
    ControlBlock<T[]>* controlBlock = new ControlBlock<T[]>(array, del);

    return ESharedPtr<T[]>(controlBlock);
}
