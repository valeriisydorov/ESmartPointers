#include "TestEUniquePtr.h"
#include "EUniquePtr.h"
#include "MakeEUnique.h"
#include "../DemoResources/DynamicResource.h"
#include "../DemoResources/DynamicArrayResource.h"
#include <cassert>
#include <iostream>


void TestEUniquePtr()
{
    {
        // EUniquePtr(PointerType ptr = nullptr, DeleterType del = Deleter()), operator==(const EUniquePtr<P, D>& lhs, std::nullptr_t)
        EUniquePtr<DynamicResource> ptr_1;
        assert(ptr_1 == nullptr);
    }

    {
        // makeEUnique(Args&&... args), EUniquePtr(EUniquePtr&& other), operator=(EUniquePtr&& rhs), operator bool(), operator->(), isValid()
        EUniquePtr<DynamicResource> ptr_2_1 = makeEUnique<DynamicResource>(13);
        EUniquePtr<DynamicResource> ptr_2_2(std::move(ptr_2_1));
        assert(ptr_2_1.isValid() == false);
        assert(ptr_2_2.isValid() == true);
        ptr_2_1 = std::move(ptr_2_2);
        assert(ptr_2_1);
        assert(!ptr_2_2);
        ptr_2_1->demo();
    }

    {
        // reset(std::nullptr_t = nullptr), reset(PointerType ptr = PointerType()), operator*()
        EUniquePtr<DynamicResource> ptr_3_1 = makeEUnique<DynamicResource>(17);
        EUniquePtr<DynamicResource> ptr_3_2 = makeEUnique<DynamicResource>(19);
        ptr_3_1.reset(nullptr);
        ptr_3_2.reset(new DynamicResource(23));
        assert(!ptr_3_1);
        assert(ptr_3_2);
        (*ptr_3_2).demo();
    }

}