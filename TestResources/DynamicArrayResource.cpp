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