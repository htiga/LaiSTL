#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>

template<typename TPair>
void PrintPair(const TPair & pair)
{
    std::cout << "{ " << pair.first << ", " << pair.second << " }" << std::endl;
}
#include "Unordered_Set.h"
#include "Unordered_Map.h"
#include <unordered_set>
#include <unordered_map>
#include <ctime>
int main()
{
    std::cout << std::boolalpha;
    {
    }
    _CrtDumpMemoryLeaks();
    return 0;
}