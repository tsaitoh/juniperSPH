#include <iostream>
#include <string>

#include "Simulation.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " particle_file config_file" << std::endl;
    }
    Simulation data = Simulation(argv[1]);
    data.useConfig(argv[2]);

    data.densityIterate();

    std::cout << "Density iteration complete" << std::endl;

    return 0;
}
