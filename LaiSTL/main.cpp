#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "Map.h"
#include <map>
#include <string>



int main()
{
    std::cout << std::boolalpha;
    {
    }
    _CrtDumpMemoryLeaks();
    return 0;
}