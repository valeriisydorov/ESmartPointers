cmake_minimum_required(VERSION 3.29)
project(ESmartPointers)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    ESmartPointers main.cpp
    ESharedPtr/TestESharedPtr.cpp
    EUniquePtr/TestEUniquePtr.cpp
    TestResources/DynamicResource.cpp
    TestResources/DynamicArrayResource.cpp
    EWeakPtr/TestEWeakPtr.cpp
)
