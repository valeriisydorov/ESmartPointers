#pragma once


template<typename T>
struct DefaultDelete
{
    void operator()(T* ptr) const
    {
        delete ptr;
    }
};

template<typename T>
struct DefaultDelete<T[]>
{
    void operator()(T* ptr) const
    {
        delete[] ptr;
    }
};