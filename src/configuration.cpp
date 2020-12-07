#include <exception>
#include <iostream>
#include <filesystem>

#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "path.hpp"
#include "configuration.hpp"

bool Configuration::initialize()
{
    return loadJson();
}

bool Configuration::loadJson()
{
    if(!PATH::isExist(mConfigurationPath))
        return PATH::outFileError(mConfigurationPath);

    boost::property_tree::ptree ptree;
    try
        {boost::property_tree::read_json(mConfigurationPath, ptree);}
    catch(const std::exception &e)
    {
        std::cerr << "loadJson() error:\n"
            "    what: " << e.what() << "\n"
            "    path: " << mConfigurationPath.string()
            << std::endl;
        return false;
    }

    bool isValid = true;
    if(auto opt = ptree.get_optional<std::string>(mOutputDirKey); opt)
        mOutputDir = opt.get();
    else
        isValid = false;

    if(auto optarr = ptree.get_child_optional(mScanningPathsKey); optarr)
    {
        for(auto &&c : optarr.get())
        {
            if(auto opt = c.second.get_optional<std::string>(""); opt)
                mScanningPaths.emplace_back(opt.get());
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        std::cerr << "loadJson() error:\n"
            "    what: failed to get some value.\n"
            "    path: " << mConfigurationPath.string()
            << std::endl;
        return false;
    }
}