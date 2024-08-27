#pragma once

#include <cstddef>
#include <utility>
#include <cassert>
#include <type_traits>
#include "DefaultDelete.h"


template<typename T, typename Deleter = DefaultDelete<T>>
class EUniquePtr
{
    template <typename P1, typename D1, typename P2, typename D2>
    friend bool operator==(const EUniquePtr<P1, D1>& lhs, const EUniquePtr<P2, D2>& rhs)
    {
        return lhs.pointer == rhs.pointer;
    }

    template <typename P, typename D>
    friend bool operator==(const EUniquePtr<P, D>& lhs, std::nullptr_t) noexcept
    {
        return lhs.pointer == nullptr;
    }

    template <typename P, typename D>
    friend bool operator==(std::nullptr_t, const EUniquePtr<P, D>& rhs) noexcept
    {
        return nullptr == rhs.pointer;
    }

public:
    using ElementType = T;
    using PointerType = ElementType*;
    using DeleterType = Deleter;

    explicit EUniquePtr(PointerType ptr = nullptr, DeleterType del = Deleter()) : pointer(ptr), deleter(del) {}
    EUniquePtr(const EUniquePtr& other) = delete;
    EUniquePtr& operator=(const EUniquePtr& rhs) = delete;
    EUniquePtr(EUniquePtr&& other) noexcept;
    EUniquePtr& operator=(EUniquePtr&& rhs) noexcept;
    ~EUniquePtr();

    void reset(PointerType ptr = PointerType()) noexcept;
    template <typename U>
    void reset(U ptr) noexcept;
    void reset(std::nullptr_t = nullptr) noexcept;

    explicit operator bool() const noexcept;
    bool isValid() const noexcept;

    typename std::add_lvalue_reference<T>::type operator*() const noexcept;
    PointerType operator->() const noexcept;

private:
    PointerType pointer;
    DeleterType deleter;

};

template<typename T, typename Deleter>
EUniquePtr<T, Deleter>::EUniquePtr(EUniquePtr&& other) noexcept
    : pointer(other.pointer)
    , deleter(std::move(other.deleter))
{
    other.pointer = nullptr;
}

template<typename T, typename Deleter>
EUniquePtr<T, Deleter>& EUniquePtr<T, Deleter>::operator=(EUniquePtr&& rhs) noexcept
{
    if (this != &rhs)
    {
        if (pointer)
        {
            deleter(pointer);
        }

        pointer = rhs.pointer;
        deleter = std::move(rhs.deleter);

        rhs.pointer = nullptr;
    }

    return *this;
}

template<typename T, typename Deleter>
EUniquePtr<T, Deleter>::~EUniquePtr()
{
    deleter(pointer);
}

template<typename T, typename Deleter>
void EUniquePtr<T, Deleter>::reset(PointerType ptr) noexcept
{
    if (pointer != ptr)
    {
        deleter(pointer);
        pointer = ptr;
    }
}

template<typename T, typename Deleter>
template<typename U>
void EUniquePtr<T, Deleter>::reset(U ptr) noexcept
{
    static_assert(std::is_convertible<U*, T*>::value, "Invalid pointer type: U* cannot be converted to T*.");

    deleter(pointer);
    pointer = ptr;
}

template<typename T, typename Deleter>
void EUniquePtr<T, Deleter>::reset(std::nullptr_t) noexcept
{
    if (pointer != nullptr)
    {
        deleter(pointer);
        pointer = nullptr;
    }
}

template<typename T, typename Deleter>
EUniquePtr<T, Deleter>::operator bool() const noexcept
{
    return pointer != nullptr;
}

template<typename T, typename Deleter>
bool EUniquePtr<T, Deleter>::isValid() const noexcept
{
    return *this;
}

template<typename T, typename Deleter>
typename std::add_lvalue_reference<T>::type EUniquePtr<T, Deleter>::operator*() const noexcept
{
    assert(pointer != nullptr && "Attempt to dereference a null pointer");

    return *pointer;
}

template<typename T, typename Deleter>
typename EUniquePtr<T, Deleter>::PointerType EUniquePtr<T, Deleter>::operator->() const noexcept
{
    assert(pointer != nullptr && "Attempt to access a member through a null pointer");

    return pointer;
}