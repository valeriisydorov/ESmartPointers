#include "DynamicResource.h"
#include <iostream>


int DynamicResource::counter = 0;

DynamicResource::DynamicResource(int value)
    : data(new int(value))
{
    ++counter;

    std::cout << "DynamicResource created. Count: " << counter <<  std::endl;
}

DynamicResource::DynamicResource(DynamicResource&& other) noexcept
    : data(other.data)
{
    other.data = nullptr;
    ++counter;

    std::cout << "DynamicResource moved." << std::endl;
}

DynamicResource& DynamicResource::operator=(DynamicResource&& rhs) noexcept
{
    if (this != &rhs)
    {
        delete data;
        data = rhs.data;
        rhs.data = nullptr;

        std::cout << "DynamicResource move-assigned." << std::endl;
    }

    return *this;
}

DynamicResource::~DynamicResource()
{
    delete data;
    --counter;

    std::cout << "DynamicResource destroyed. Count: " << counter << std::endl;
}

void DynamicResource::demo()
{
    if (data)
    {
        std::cout << "Resource data is initialized. Data: " << *data << std::endl;
    }
    else
    {
        std::cout << "Resource data is not initialized." << std::endl;
    }
}