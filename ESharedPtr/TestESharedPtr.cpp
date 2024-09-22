#include "TestESharedPtr.h"
#include "ESharedPtr.h"
#include "MakeEShared.h"
#include "../EUniquePtr/MakeEUnique.h"
#include "../TestResources/DynamicArrayResource.h"
#include "../EWeakPtr/EWeakPtr.h"
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
        ESharedPtr<DynamicArrayResource> ptr_2_1 = makeEShared<DynamicArrayResource>(10, 11);
        ESharedPtr<DynamicArrayResource> ptr_2_2(ptr_2_1);
        ESharedPtr<DynamicArrayResource> ptr_2_3 = ptr_2_2;
        assert(ptr_2_1 == ptr_2_2);
        assert(ptr_2_2 == ptr_2_3);
        ptr_2_3->demo();
    }

    {
        // ESharedPtr(ESharedPtr&& other), operator=(ESharedPtr&& rhs), isValid(), operator*()
        ESharedPtr<DynamicArrayResource> ptr_3_1 = makeEShared<DynamicArrayResource>(10, 13);
        ESharedPtr<DynamicArrayResource> ptr_3_2(std::move(ptr_3_1));
        assert(ptr_3_1.isValid() == false);
        assert(ptr_3_2.isValid() == true);
        ESharedPtr<DynamicArrayResource> ptr_3_3 = std::move(ptr_3_2);
        assert(ptr_3_2.isValid() == false);
        assert(ptr_3_3.isValid() == true);
        (*ptr_3_3).demo();
    }

    {
        // ESharedPtr(EUniquePtr<T, Deleter>&& other);
        EUniquePtr<DynamicArrayResource> ptr_4_1 = makeEUnique<DynamicArrayResource>(10, 17);
        ESharedPtr<DynamicArrayResource> ptr_4_2(std::move(ptr_4_1));
        assert(ptr_4_1.isValid() == false);
        assert(ptr_4_2.isValid() == true);
        ptr_4_2->demo();
    }

    {
        // reset(), reset(Y* ptr)
        ESharedPtr<DynamicArrayResource> ptr_5_1 = makeEShared<DynamicArrayResource>(10, 19);
        ESharedPtr<DynamicArrayResource> ptr_5_2 = makeEShared<DynamicArrayResource>(10, 23);
        ptr_5_1.reset();
        ptr_5_2.reset(new DynamicArrayResource(10, 29));
        assert(ptr_5_1.isValid() == false);
        assert(ptr_5_2.isValid() == true);
        ptr_5_2->demo();
    }

    {
        // toWeak(), ESharedPtr(const EWeakPtr<Y>& other)
        ESharedPtr<DynamicArrayResource> ptr_6_1 = makeEShared<DynamicArrayResource>(10, 11);
        EWeakPtr<DynamicArrayResource> ptr_6_2 = ptr_6_1.toWeak();
        ESharedPtr<DynamicArrayResource> ptr_6_3(ptr_6_2);
        assert(ptr_6_1.isValid() == true);
        assert(ptr_6_2.isValid() == true);
        assert(ptr_6_3.isValid() == true);
        ptr_6_3->demo();
    }
}