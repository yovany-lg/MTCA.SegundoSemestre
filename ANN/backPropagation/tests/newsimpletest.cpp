/* 
 * File:   newsimpletest.cpp
 * Author: yovany
 *
 * Created on May 11, 2014, 10:53:46 PM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */

void initialize(int layers, int neurons[], int inputs, Layer** layersList);

void testInitialize() {
    int layers = 2;
    int* neurons = [5,6];
    int inputs = 5;
    Layer** layersList;
    initialize(layers, neurons, inputs, layersList);
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testInitialize (newsimpletest) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testInitialize (newsimpletest)" << std::endl;
    testInitialize();
    std::cout << "%TEST_FINISHED% time=0 testInitialize (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

