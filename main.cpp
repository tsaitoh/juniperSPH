#include "kernel.h"
#include <iostream>
#include <string>
#include <cmath>

int main()
{
    Kernel kernel = Kernel();

    for (float q = 0; q <= 2; q += 0.25){
        std::cout << "f(" << q << ") = " << kernel.valueAt(q) << std::endl;
    }

    return 0;
}
