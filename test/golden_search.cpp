#include <iostream>
#define DEBUG_GOLDEN_SERACH
#include <basic/golden_search.hpp>
using Basic::golden_search;

int main()
{
    golden_search([](double t){ return t*t;}, -1.3, 1.0);
}
