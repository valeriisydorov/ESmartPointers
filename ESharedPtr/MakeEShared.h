#pragma once

#include <cstddef>
#include <utility>
#include "ESharedPtr.h"


template<typename T, typename... Args>
ESharedPtr<T> makeEShared(Args&&... args)
{
    T* object = new T(std::forward<Args>(args)...);
    ControlBlock<T>* controlBlock = new ControlBlock<T>(object, DefaultDelete<T>(), 1);

    return ESharedPtr<T>(controlBlock);
}

template <typename T>
ESharedPtr<T[]> makeEShared(std::size_t size)
{
    T* array = new T[size];
    ControlBlock<T[]>* controlBlock = new ControlBlock<T[]>(array, DefaultDelete<T[]>(), 1);

    return ESharedPtr<T[]>(controlBlock);
}