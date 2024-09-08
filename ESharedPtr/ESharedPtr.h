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
    friend bool operator==(const ESharedPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }

    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, std::nullptr_t) noexcept
    {
        return lhs.controlBlock == nullptr;
    }

    template <typename P>
    friend bool operator==(std::nullptr_t, const ESharedPtr<P>& rhs) noexcept
    {
        return nullptr == rhs.controlBlock;
    }

    template<typename U, typename... Args>
    friend ESharedPtr<U> makeEShared(Args&&... args);

    template<typename U, typename Deleter, typename... Args>
    friend ESharedPtr<U> makeEShared(Deleter del, Args&&... args);

    template <typename U>
    friend ESharedPtr<U[]> makeEShared(std::size_t size);

    template <typename U, typename Deleter>
    friend ESharedPtr<U[]> makeEShared(std::size_t size, Deleter del);

public:
    using ElementType = T;
    using PointerType = ElementType*;
    using ControlBlockPointerType = ControlBlock<ElementType>*;
//    using WeakPointerType = EWeakPtr<ElementType>;

    constexpr ESharedPtr() noexcept
        : controlBlock(nullptr)
    {
    }
    ESharedPtr(const ESharedPtr& other);
    ESharedPtr(ESharedPtr&& other) noexcept;

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
    ControlBlockPointerType controlBlock;

    explicit ESharedPtr(ControlBlockPointerType control)
        : controlBlock(control)
    {
    }
    void checkControlBlockAndRelease();
};

template <typename T>
ESharedPtr<T>::ESharedPtr(const ESharedPtr& other)
{
    controlBlock = other.controlBlock;
    if (controlBlock != nullptr)
    {
        controlBlock->incrementShared();
    }
}

template <typename T>
ESharedPtr<T>::ESharedPtr(ESharedPtr&& other) noexcept : controlBlock(other.controlBlock)
{
    other.controlBlock = nullptr;
}

template <typename T>
template <typename Deleter>
ESharedPtr<T>::ESharedPtr(EUniquePtr<T, Deleter>&& other)
{
    PointerType ptr = other.operator->();
    if (ptr != nullptr)
    {
        controlBlock = new ControlBlock<T, Deleter>(ptr, other.getDeleter(), 1);

        other.reset(nullptr);
    }
    else
    {
        controlBlock = nullptr;
    }
}

template<typename T>
void ESharedPtr<T>::reset() noexcept
{
    checkControlBlockAndRelease();

    controlBlock = nullptr;
}

template<typename T>
template<typename Y>
void ESharedPtr<T>::reset(Y* ptr)
{
    checkControlBlockAndRelease();

    if (ptr == nullptr)
    {
        controlBlock = nullptr;
    }
    else
    {
        controlBlock = new ControlBlock<Y>(ptr, DefaultDelete<Y>(), 1);
    }
}

template<typename T>
template<typename Y, typename Deleter>
void ESharedPtr<T>::reset(Y* ptr, Deleter del)
{
    checkControlBlockAndRelease();

    if (ptr == nullptr)
    {
        controlBlock = nullptr;
    }
    else
    {
        controlBlock = new ControlBlock<Y, Deleter>(ptr, del, 1);
    }
}

template<typename T>
ESharedPtr<T>::operator bool() const noexcept
{
    return controlBlock != nullptr;
}

template<typename T>
bool ESharedPtr<T>::isValid() const noexcept
{
    return operator bool();
}

template<typename T>
typename ESharedPtr<T>::ElementType& ESharedPtr<T>::operator*() const noexcept
{
    assert(controlBlock != nullptr && controlBlock->getObject() != nullptr && \
        "ESharedPtr: Attempt to dereference a null pointer");

    return *(controlBlock->getObject());
}

template<typename T>
typename ESharedPtr<T>::PointerType ESharedPtr<T>::operator->() const noexcept
{
    assert(controlBlock != nullptr && controlBlock->getObject() != nullptr && \
        "ESharedPtr: Attempt to dereference a null pointer");

    return controlBlock->getObject();
}

template<typename T>
void ESharedPtr<T>::checkControlBlockAndRelease()
{
    if (controlBlock != nullptr)
    {
        controlBlock->releaseShared();
    }
}