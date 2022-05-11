#include <iostream>

#include "path.hpp"
#include "analyzer.hpp"
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

    if(std::filesystem::exists(Configuration::outputDir()))
        std::filesystem::remove_all(Configuration::outputDir());
    if(!PATH::isValid(Configuration::outputDir(), std::filesystem::file_type::directory))
        return PATH::outFileError(Configuration::outputDir());

    return true;
}

bool Controller::execute()
{
    for(auto &&path : Configuration::scanningPaths())
    {
        if(!PATH::isExist(path, std::filesystem::file_type::directory))
        {
            std::cerr << "file warning:\n"
                "    what: scanning path is invalid.\n"
                "    path: " << path.string() << "\n"
                "    approach: ignore this path.\n"
                << std::endl;
            continue;
        }

        for(auto &&de : std::filesystem::directory_iterator(path))
            Analyzer(de.path()).analyze();
    }

    return true;
}