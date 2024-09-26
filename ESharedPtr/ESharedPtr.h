#pragma once


#include <cstddef>
#include "../CommonElements/DefaultDelete.h"
#include "../CommonElements/ControlBlock.h"
#include "../EUniquePtr/EUniquePtr.h"


template <typename T>
class EWeakPtr;

template <typename T>
class ESharedPtr
{
    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }

    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, std::nullptr_t np) noexcept
    {
        return lhs.controlBlock == np;
    }

    template <typename P>
    friend bool operator==(std::nullptr_t np, const ESharedPtr<P>& rhs) noexcept
    {
        return np == rhs.controlBlock;
    }

    template <typename P>
    friend bool operator!=(const ESharedPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept
    {
        return !(lhs.controlBlock == rhs.controlBlock);
    }

    template <typename P>
    friend bool operator!=(const ESharedPtr<P>& lhs, std::nullptr_t np) noexcept
    {
        return !(lhs.controlBlock == np);
    }

    template <typename P>
    friend bool operator!=(std::nullptr_t np, const ESharedPtr<P>& rhs) noexcept
    {
        return !(np == rhs.controlBlock);
    }

    template<typename U, typename... Args>
    friend ESharedPtr<U> makeEShared(Args&&... args);

    template<typename U, typename Deleter, typename... Args>
    friend ESharedPtr<U> makeEShared(Deleter del, Args&&... args);

    template <typename U>
    friend ESharedPtr<U[]> makeEShared(std::size_t size);

    template <typename U, typename Deleter>
    friend ESharedPtr<U[]> makeEShared(std::size_t size, Deleter del);

    template<typename Y>
    friend class EWeakPtr;

public:
    using ElementType = T;
    using PointerType = ElementType*;
    using ControlBlockPointerType = ControlBlock<ElementType>*;
    using EWeakPointerType = EWeakPtr<ElementType>;

    constexpr ESharedPtr() noexcept
        : controlBlock(nullptr)
    {
    }
    ESharedPtr(const ESharedPtr& other);
    ESharedPtr(ESharedPtr&& other) noexcept;

    template <typename Y>
    explicit ESharedPtr(const EWeakPtr<Y>& other);
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
    EWeakPointerType toWeak() const;

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
    void release();
    void initialize(ControlBlockPointerType otherControlBlock) noexcept;
};

template <typename T>
ESharedPtr<T>::ESharedPtr(const ESharedPtr& other)
{
    initialize(other.controlBlock);
}

template <typename T>
template <typename Y>
ESharedPtr<T>::ESharedPtr(const EWeakPtr<Y>& other)
{
    static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

    initialize(other.controlBlock);
}

template <typename T>
ESharedPtr<T>::ESharedPtr(ESharedPtr&& other) noexcept
    : controlBlock(other.controlBlock)
{
    other.controlBlock = nullptr;
}

template <typename T>
template <typename Deleter>
ESharedPtr<T>::ESharedPtr(EUniquePtr<T, Deleter>&& other)
    : controlBlock(nullptr)
{
    if (other.operator->() != nullptr)
    {
        controlBlock = new ControlBlock<T, Deleter>(std::move(other));

        other.pointer = nullptr;
    }
    else
    {
        controlBlock = nullptr;
    }
}

template <typename T>
ESharedPtr<T>& ESharedPtr<T>::operator=(const ESharedPtr& rhs)
{
    if (this != &rhs)
    {
        release();

        initialize(rhs.controlBlock);
    }

    return *this;
}

template <typename T>
ESharedPtr<T>& ESharedPtr<T>::operator=(ESharedPtr&& rhs) noexcept
{
    if (this != &rhs)
    {
        release();

        controlBlock = rhs.controlBlock;
        rhs.controlBlock = nullptr;
    }

    return *this;
}

template <typename T>
ESharedPtr<T>::~ESharedPtr()
{
    release();
}

template<typename T>
void ESharedPtr<T>::reset() noexcept
{
    release();

    controlBlock = nullptr;
}

template<typename T>
template<typename Y>
void ESharedPtr<T>::reset(Y* ptr)
{
    release();

    if (ptr == nullptr)
    {
        controlBlock = nullptr;
    }
    else
    {
        static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

        EUniquePtr<T, DefaultDelete<Y>> uniquePtr(static_cast<T*>(ptr), DefaultDelete<Y>());
        controlBlock = new ControlBlock<T, DefaultDelete<Y>>(std::move(uniquePtr));
    }
}

template<typename T>
template<typename Y, typename Deleter>
void ESharedPtr<T>::reset(Y* ptr, Deleter del)
{
    release();

    if (ptr == nullptr)
    {
        controlBlock = nullptr;
    }
    else
    {
        static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

        EUniquePtr<T, Deleter> uniquePtr(static_cast<T*>(ptr), del);
        controlBlock = new ControlBlock<Y, Deleter>(std::move(uniquePtr));
    }
}

template<typename T>
typename ESharedPtr<T>::EWeakPointerType ESharedPtr<T>::toWeak() const
{
    if (controlBlock != nullptr)
    {
        controlBlock->incrementWeak();

        return EWeakPtr<T>(controlBlock);
    }

    return EWeakPtr<T>();
}

template<typename T>
ESharedPtr<T>::operator bool() const noexcept
{
    return isValid();
}

template<typename T>
bool ESharedPtr<T>::isValid() const noexcept
{
    return controlBlock != nullptr;
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
void ESharedPtr<T>::release()
{
    if (controlBlock != nullptr)
    {
        controlBlock->releaseShared();
    }
}

template<typename T>
void ESharedPtr<T>::initialize(ControlBlockPointerType otherControlBlock) noexcept
{
    controlBlock = otherControlBlock;

    if (controlBlock != nullptr)
    {
        controlBlock->incrementShared();
    }
}