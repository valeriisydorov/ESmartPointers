#pragma once


#include <cstddef>
#include "../CommonElements/ControlBlock.h"
//#include "../ESharedPtr/ESharedPtr.h"



template <typename T>
class ESharedPtr;

template <typename T>
class EWeakPtr
{
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, const EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, std::nullptr_t) noexcept;
    template <typename P>
    friend bool operator==(std::nullptr_t, const EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, P* rhs) noexcept;
    template <typename P>
    friend bool operator==(P* lhs, EWeakPtr<P>& rhs) noexcept;

    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, const EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, std::nullptr_t) noexcept;
    template <typename P>
    friend bool operator!=(std::nullptr_t, const EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator!=(const ESharedPtr<P>& lhs, EWeakPtr<P>& rhs) noexcept;
    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, P* rhs) noexcept;
    template <typename P>
    friend bool operator!=(P* lhs, EWeakPtr<P>& rhs) noexcept;

public:
    using ElementType = T;
    using PointerType = ElementType*;
    using ControlBlockPointerType = ControlBlock<ElementType>*;
    using ESharedPointerType = ESharedPtr<ElementType>;

    constexpr EWeakPtr() noexcept;
    EWeakPtr(const EWeakPtr& other) noexcept;
    template <class Y>
    EWeakPtr(const EWeakPtr<Y>& other) noexcept;
    template<class Y>
    EWeakPtr( const ESharedPtr<Y>& other) noexcept;
    EWeakPtr(EWeakPtr&& other) noexcept;
    template<class Y>
    EWeakPtr(EWeakPtr<Y>&& other) noexcept;
    EWeakPtr& operator=(const EWeakPtr& rhs) noexcept;
    template<class Y>
    EWeakPtr& operator=(const EWeakPtr<Y>& rhs) noexcept;
    template<class Y>
    EWeakPtr& operator=(const ESharedPtr<Y>& rhs) noexcept;
    EWeakPtr& operator=(EWeakPtr&& rhs) noexcept;
    template<class Y>
    EWeakPtr& operator=(EWeakPtr<Y>&& rhs) noexcept;
    ~EWeakPtr();

    void reset() noexcept;

    explicit operator bool() const noexcept;
    bool isValid() const noexcept;

    ESharedPointerType lock() const noexcept;

private:
    ControlBlockPointerType controlBlock;

};


template<typename T>
EWeakPtr<T>::operator bool() const noexcept
{
    return isValid();
}

template<typename T>
bool EWeakPtr<T>::isValid() const noexcept
{
    return controlBlock != nullptr;
}
