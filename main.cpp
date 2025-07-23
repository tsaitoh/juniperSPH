#include "kernel.h"
#include <iostream>
#include <string>

#include "SimData.h"

int main()
{
    SimData data = SimData(".//notebooks//ot_00003.csv");
    Kernel kernel = Kernel();

    std::cout << data.densityAt(0, kernel) << std::endl;
    std::cout << data.densityAt(1, kernel) << std::endl;

    return 0;
}
