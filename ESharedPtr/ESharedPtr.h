#pragma once


#include <cstddef>
#include "../CommonElements/DefaultDelete.h"
#include "../CommonElements/ControlBlock.h"
#include "../EUniquePtr/EUniquePtr.h"
//  #include "../EWeakPtr/EWeakPtr.h"


template <typename T>
class ESharedPtr
{
    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept;

    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, std::nullptr_t) noexcept;

    template <typename P>
    friend bool operator==(std::nullptr_t, const ESharedPtr<P>& rhs) noexcept;

public:
    using ElementType = T;
    using PointerType = ElementType*;
    using ControlBlockPointerType = ControlBlock<ElementType>*;
//    using WeakPointerType = EWeakPtr<ElementType>;

    constexpr ESharedPtr() noexcept
        : control(nullptr)
        , pointer(nullptr)
    {
    }
    constexpr ESharedPtr(std::nullptr_t) noexcept
        : ESharedPtr()
    {
    }
    explicit ESharedPtr(ControlBlockPointerType ctrl)
        : control(ctrl)
        , pointer(ctrl ? ctrl->getObject() : nullptr)
    {
    }
    ESharedPtr(const ESharedPtr& other);
    ESharedPtr(ESharedPtr&& other) noexcept;

    template <typename Deleter>
    ESharedPtr(PointerType ptr, Deleter del);

//    template <typename Y>
//    explicit ESharedPtr(const EWeakPtr<Y>& other);
    template <typename Deleter>
    ESharedPtr(EUniquePtr<T, Deleter>&& other);

    ESharedPtr& operator=(const ESharedPtr& rhs);
    ESharedPtr& operator=(ESharedPtr&& rhs) noexcept;
    ~ESharedPtr();

    void reset() noexcept;
    template<typename Y>
    void reset(Y* ptr);
    template<typename Y, typename Deleter>
    void reset(Y* ptr, Deleter del);
//    WeakPointerType toWeak() const;

    explicit operator bool() const noexcept;
    bool isValid() const noexcept;

    ElementType& operator*() const noexcept;
    PointerType operator->() const noexcept;

private:
    ControlBlockPointerType control;
    PointerType pointer;

};


template<typename T>
ESharedPtr<T>::operator bool() const noexcept
{
    return pointer != nullptr;
}

template<typename T>
bool ESharedPtr<T>::isValid() const noexcept
{
    return operator bool();
}