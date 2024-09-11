#include "DynamicArrayResource.h"
#include <iostream>


int DynamicArrayResource::counter = 0;

DynamicArrayResource::DynamicArrayResource(std::size_t len, int val)
    : length(len)
    , value(nullptr)
{
    if (length > 0)
    {
        value = new int[length];
        for (std::size_t i = 0; i < length; ++i)
        {
            value[i] = val;
        }
    }
    else
    {
        throw std::invalid_argument("Array size must be greater than zero.");
    }

    ++counter;
    std::cout << "DynamicArrayResource created. Count: " << counter << std::endl;
}

DynamicArrayResource::DynamicArrayResource(DynamicArrayResource&& other) noexcept
    : value(other.value)
    , length(other.length)
{
    other.value = nullptr;
    other.length = 0;

    std::cout << "DynamicArrayResource moved." << std::endl;
}

DynamicArrayResource& DynamicArrayResource::operator=(DynamicArrayResource&& rhs) noexcept
{
    if (this != &rhs)
    {
        delete[] value;

        value = rhs.value;
        length = rhs.length;

        rhs.value = nullptr;
        rhs.length = 0;

        std::cout << "DynamicArrayResource move-assigned." << std::endl;
    }

    return *this;
}

DynamicArrayResource::~DynamicArrayResource()
{
    delete[] value;
    --counter;

    std::cout << "DynamicArrayResource destroyed. Count: " << counter << std::endl;
}

void DynamicArrayResource::demo()
{
    if (value)
    {
        std::cout << "ArrayResource size: " << length << std::endl;
        std::cout << "ArrayResource contents: ";
        for (std::size_t i = 0; i < length; ++i)
        {
            std::cout << value[i] << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "ArrayResource value is not initialized." << std::endl;
    }
}