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
    if(!PATH::isExist(mScanningPathFile))
        return PATH::outFileError(mScanningPathFile);

    boost::property_tree::ptree ptree;
    try
        {boost::property_tree::read_json(mScanningPathFile, ptree);}
    catch(const std::exception &e)
    {
        std::cerr << "loadJson() error:\n"
            "    what: " << e.what() << "\n"
            "    path: " << mScanningPathFile.string()
            << std::endl;
        return false;
    }

    if(auto optarr = ptree.get_child_optional(mScanningPathsKey); optarr)
    {
        for(auto &&c : optarr.get())
        {
            if(auto opt = c.second.get_optional<std::string>(""); opt)
                mScanningPaths.emplace_back(opt.get());
        }
    }
    

    return true;
}