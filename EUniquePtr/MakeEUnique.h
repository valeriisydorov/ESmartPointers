#pragma once


#include <utility>
#include "EUniquePtr.h"


template<typename T, typename... Args>
EUniquePtr<T> makeEUnique(Args&&... args)
{
    return EUniquePtr<T>(new T(std::forward<Args>(args)...));
}