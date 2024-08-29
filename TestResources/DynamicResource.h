#pragma once


class DynamicResource
{
public:
    static int counter;

    explicit DynamicResource(int value = 0);
    DynamicResource(const DynamicResource&) = delete;
    DynamicResource(DynamicResource&& other) noexcept;
    DynamicResource& operator=(const DynamicResource&) = delete;
    DynamicResource& operator=(DynamicResource&&) noexcept;
    ~DynamicResource();

    void demo();

private:
    int* data;

};
