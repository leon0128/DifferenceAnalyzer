#include <iostream>
#include "configuration.hpp"
#include "controller.hpp"

Controller::Controller()
{
}

bool Controller::initialize()
{
    if(!Configuration::initialize())
    {
        std::cerr << "Controller::initialize() error:\n"
            "    what: failed to call Configuration::initialize()\n"
            << std::flush;
        return false;
    }

    return true;
}

bool Controller::execute()
{
    return true;
}