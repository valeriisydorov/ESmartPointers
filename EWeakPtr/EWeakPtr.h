#pragma once


#include <cstddef>
#include "../CommonElements/ControlBlock.h"


template <typename T>
class ESharedPtr;

template <typename T>
class EWeakPtr
{
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, const EWeakPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, std::nullptr_t np) noexcept
    {
        return lhs.controlBlock == np;
    }
    template <typename P>
    friend bool operator==(std::nullptr_t np, const EWeakPtr<P>& rhs) noexcept
    {
        return np == rhs.controlBlock;
    }
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }
    template <typename P>
    friend bool operator==(const ESharedPtr<P>& lhs, EWeakPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }

    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, const EWeakPtr<P>& rhs) noexcept
    {
        return !(lhs.controlBlock == rhs.controlBlock);
    }
    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, std::nullptr_t np) noexcept
    {
        return !(lhs.controlBlock == np);
    }
    template <typename P>
    friend bool operator!=(std::nullptr_t np, const EWeakPtr<P>& rhs) noexcept
    {
        return !(np == rhs.controlBlock);
    }
    template <typename P>
    friend bool operator!=(const EWeakPtr<P>& lhs, const ESharedPtr<P>& rhs) noexcept
    {
        return !(lhs.controlBlock == rhs.controlBlock);
    }
    template <typename P>
    friend bool operator!=(const ESharedPtr<P>& lhs, EWeakPtr<P>& rhs) noexcept
    {
        return !(lhs.controlBlock == rhs.controlBlock);
    }

    template<typename Y>
    friend class ESharedPtr;

public:
    using ElementType = T;
    using ControlBlockPointerType = ControlBlock<ElementType>*;
    using ESharedPointerType = ESharedPtr<ElementType>;

    constexpr EWeakPtr() noexcept
        : controlBlock(nullptr)
    {
    }
    EWeakPtr(const EWeakPtr& other) noexcept;
    template <typename Y>
    EWeakPtr(const EWeakPtr<Y>& other) noexcept;
    template <typename Y>
    EWeakPtr(const ESharedPtr<Y>& other) noexcept;
    EWeakPtr(EWeakPtr&& other) noexcept;
    template <typename Y>
    EWeakPtr(EWeakPtr<Y>&& other) noexcept;
    EWeakPtr& operator=(const EWeakPtr& rhs) noexcept;
    template <typename Y>
    EWeakPtr& operator=(const EWeakPtr<Y>& rhs) noexcept;
    template <typename Y>
    EWeakPtr& operator=(const ESharedPtr<Y>& rhs) noexcept;
    EWeakPtr& operator=(EWeakPtr&& rhs) noexcept;
    template <typename Y>
    EWeakPtr& operator=(EWeakPtr<Y>&& rhs) noexcept;
    ~EWeakPtr();

    void reset() noexcept;

    explicit operator bool() const noexcept;
    bool isValid() const noexcept;

    ESharedPointerType lock() const noexcept;

private:
    ControlBlockPointerType controlBlock;

    explicit EWeakPtr(ControlBlockPointerType control)
        : controlBlock(control)
    {
    }
    void initialize(ControlBlockPointerType otherControlBlock) noexcept;
};


template <typename T>
EWeakPtr<T>::EWeakPtr(const EWeakPtr& other) noexcept
{
    initialize(other.controlBlock);
}

template <typename T>
template <typename Y>
EWeakPtr<T>::EWeakPtr(const EWeakPtr<Y>& other) noexcept
{
    static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

    initialize(other.controlBlock);
}

template <typename T>
template <typename Y>
EWeakPtr<T>::EWeakPtr(const ESharedPtr<Y>& other) noexcept
{
    static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

    initialize(other.controlBlock);
}

template <typename T>
EWeakPtr<T>::EWeakPtr(EWeakPtr&& other) noexcept
    : controlBlock(other.controlBlock)
{
    other.controlBlock = nullptr;
}

template <typename T>
template <typename Y>
EWeakPtr<T>::EWeakPtr(EWeakPtr<Y>&& other) noexcept
    : controlBlock(other.controlBlock)
{
    static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

    other.controlBlock = nullptr;
}

template <typename T>
EWeakPtr<T>& EWeakPtr<T>::operator=(const EWeakPtr& rhs) noexcept
{
    if (this != &rhs)
    {
        reset();

        initialize(rhs.controlBlock);
    }

    return *this;
}

template <typename T>
template <typename Y>
EWeakPtr<T>& EWeakPtr<T>::operator=(const ESharedPtr<Y>& rhs) noexcept
{
    static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

    reset();

    initialize(rhs.controlBlock);

    return *this;
}

template <typename T>
template <typename Y>
EWeakPtr<T>& EWeakPtr<T>::operator=(EWeakPtr<Y>&& rhs) noexcept
{
    if (this != &rhs)
    {
        static_assert(std::is_convertible<Y*, T*>::value, "Type Y* must be convertible to T*");

        reset();

        controlBlock = rhs.controlBlock;
        rhs.controlBlock = nullptr;
    }

    return *this;
}

template <typename T>
EWeakPtr<T>::~EWeakPtr()
{
    reset();
}

template <typename T>
void EWeakPtr<T>::reset() noexcept
{
    if (controlBlock != nullptr)
    {
        controlBlock->releaseWeak();
        controlBlock = nullptr;
    }
}

template <typename T>
EWeakPtr<T>::operator bool() const noexcept
{
    return isValid();
}

template <typename T>
bool EWeakPtr<T>::isValid() const noexcept
{
    return controlBlock != nullptr && controlBlock->getSharedCount() != 0;
}

template <typename T>
typename EWeakPtr<T>::ESharedPointerType EWeakPtr<T>::lock() const noexcept
{
    if (isValid() == true)
    {
        controlBlock->incrementShared();

        return ESharedPtr<T>(controlBlock);
    }

    return ESharedPtr<T>();
}

template<typename T>
void EWeakPtr<T>::initialize(ControlBlockPointerType otherControlBlock) noexcept
{
    controlBlock = otherControlBlock;

    if (controlBlock != nullptr)
    {
        controlBlock->incrementWeak();
    }
}








