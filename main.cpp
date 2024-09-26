#include "EUniquePtr/TestEUniquePtr.h"
#include "ESharedPtr/TestESharedPtr.h"
#include "EWeakPtr/TestEWeakPtr.h"


int main()
{
    TestEUniquePtr();
    TestESharedPtr();
    TestEWeakPtr();

    return 0;
}
