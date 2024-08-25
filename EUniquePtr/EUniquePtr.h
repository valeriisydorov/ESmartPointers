#pragma once

#include <cstddef>
#include <type_traits>
#include "DefaultDelete.h"


template<typename T, typename Deleter = DefaultDelete<T>>
class EUniquePtr
{
    template <typename T1, typename D1, typename T2, typename D2>
    friend bool operator==(const EUniquePtr<T1, D1>& lhs, const EUniquePtr<T2, D2>& rhs);
    template <typename T1, typename D1, typename T2, typename D2>
    friend bool operator==(const EUniquePtr<T1, D1>& lhs, std::nullptr_t) noexcept;
    template <typename T1, typename D1, typename T2, typename D2>
    friend bool operator==(std::nullptr_t, const EUniquePtr<T2, D2>& rhs) noexcept;

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

    typename std::add_lvalue_reference<ElementType> operator*() const noexcept;
    PointerType operator->() const noexcept;

private:
    PointerType pointer;
    DeleterType deleter;

};
