#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>

template<typename TPair>
void PrintPair(const TPair & pair) noexcept
{
    std::cout << "{ " << pair.first << ", " << pair.second << " }";
}

int main()
{
    std::cout << std::boolalpha;
    {
    }
    _CrtDumpMemoryLeaks();
    return 0;
}