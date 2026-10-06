#include "core/app.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
    try {
        lume::App app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}