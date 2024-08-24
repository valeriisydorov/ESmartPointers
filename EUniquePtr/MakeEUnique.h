#pragma once

#include <utility>
#include "EUniquePtr.h"

template<typename T, typename... Args>
EUniquePtr<T> MakeEUnique(Args&&... args)
{
    return EUniquePtr<T>(new T(std::forward<Args>(args)...));
}