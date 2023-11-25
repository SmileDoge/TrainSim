#include <iostream>
#include "engine.hpp"

int main()
{
    std::cout << "Initializing Engine..." << std::endl;

    IEngine* engine = CreateEngine();

    std::cout << "Engine: " << engine << std::endl;

    engine->SomeFunc();

    engine = CreateEngine();

    FreeEngine();
}