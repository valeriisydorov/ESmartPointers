#pragma once


#include <cstddef>


class DynamicArrayResource
{
public:
    static int counter;

    explicit DynamicArrayResource(std::size_t len, int val = 0);
    DynamicArrayResource(const DynamicArrayResource&) = delete;
    DynamicArrayResource(DynamicArrayResource&& other) noexcept;
    DynamicArrayResource& operator=(const DynamicArrayResource&) = delete;
    DynamicArrayResource& operator=(DynamicArrayResource&& rhs) noexcept;
    ~DynamicArrayResource();

    void demo();

private:
    int* value;
    std::size_t length;

};
