#include "TestESharedPtr.h"
#include "ESharedPtr.h"
#include "MakeEShared.h"
#include "../TestResources/DynamicArrayResource.h"
#include <cassert>


void TestESharedPtr()
{
    {
        // ESharedPtr(), bool(), operator==(const ESharedPtr<P>& lhs, std::nullptr_t)
        ESharedPtr<DynamicArrayResource> ptr_1;
        assert(!ptr_1);
        assert(ptr_1 == nullptr);
    }

    {
        // makeEShared(Args&&... args), ESharedPtr(const ESharedPtr& other), operator=(const ESharedPtr& rhs), operator->()
        ESharedPtr<DynamicArrayResource> ptr_2_1 = makeEShared<DynamicArrayResource>(13);
        ESharedPtr<DynamicArrayResource> ptr_2_2(ptr_2_1);
        ESharedPtr<DynamicArrayResource> ptr_2_3 = ptr_2_2;
        assert(ptr_2_1 == ptr_2_2);
        assert(ptr_2_2 == ptr_2_3);
        ptr_2_3->demo();
    }

    {
        // ESharedPtr(ESharedPtr&& other), operator=(ESharedPtr&& rhs), isValid(), operator*()
        ESharedPtr<DynamicArrayResource> ptr_3_1 = makeEShared<DynamicArrayResource>(17);
        ESharedPtr<DynamicArrayResource> ptr_3_2(std::move(ptr_3_1));
        assert(ptr_3_1.isValid() == false);
        assert(ptr_3_2.isValid() == true);
        ESharedPtr<DynamicArrayResource> ptr_3_3 = std::move(ptr_3_2);
        assert(ptr_3_2.isValid() == false);
        assert(ptr_3_3.isValid() == true);
        (*ptr_3_3).demo();
    }
}