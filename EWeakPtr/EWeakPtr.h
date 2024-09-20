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
    friend bool operator==(const EWeakPtr<P>& lhs, const EWeakPtr<P>& rhs) noexcept
    {
        return lhs.controlBlock == rhs.controlBlock;
    }
    template <typename P>
    friend bool operator==(const EWeakPtr<P>& lhs, std::nullptr_t) noexcept
    {
        return lhs.controlBlock == nullptr;
    }
    template <typename P>
    friend bool operator==(std::nullptr_t, const EWeakPtr<P>& rhs) noexcept
    {
        return nullptr == rhs.controlBlock;
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
    friend bool operator!=(const EWeakPtr<P>& lhs, std::nullptr_t) noexcept
    {
        return !(lhs.controlBlock == nullptr);
    }
    template <typename P>
    friend bool operator!=(std::nullptr_t, const EWeakPtr<P>& rhs) noexcept
    {
        return !(nullptr == rhs.controlBlock);
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

public:
    using ElementType = T;
    using PointerType = ElementType*;
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

};

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
    return controlBlock != nullptr;
}
