#include "TestEWeakPtr.h"
#include "EWeakPtr.h"
#include "../ESharedPtr/ESharedPtr.h"
#include "../ESharedPtr/MakeEShared.h"
#include "../TestResources/DynamicResource.h"
#include <cassert>
#include <iostream>


void TestEWeakPtr()
{
    // EWeakPtr(const ESharedPtr<Y>& other)
    ESharedPtr<DynamicResource> sp_1 = makeEShared<DynamicResource>(13);
    EWeakPtr<DynamicResource> wp(sp_1);
    assert(sp_1.isValid() == true);
    assert(wp.isValid() == true);

    // lock()
    ESharedPtr<DynamicResource> sp_2 = wp.lock();
    assert(sp_2.isValid() == true);
    assert(wp.isValid() == true);
    sp_2->demo();

    sp_1.reset();
    assert(sp_1.isValid() == false);
    assert(wp.isValid() == true);
    sp_2.reset();
    assert(sp_2.isValid() == false);
    assert(wp.isValid() == false);
}