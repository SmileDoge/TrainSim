#include "cengine.hpp"
#include <iostream>

CEngine::CEngine()
{
    std::cout << "CEngine Created!" << std::endl;
}

CEngine::~CEngine()
{
    std::cout << "CEngine Destroyed!" << std::endl;
}

void CEngine::SomeFunc()
{
    std::cout << "CEngine Some Func!" << std::endl;
}