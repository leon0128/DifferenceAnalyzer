#include "path.hpp"

#include <filesystem>
#include <iostream>

namespace PATH
{

bool isValid(const std::filesystem::path &path
    , std::filesystem::file_type type)
{
    if(type == std::filesystem::file_type::regular)
    {
        if(isValid(path.parent_path(), std::filesystem::file_type::directory)
            && !isExist(path, std::filesystem::file_type::directory))
            return true;
        else
            return false;
    }
    else if(type == std::filesystem::file_type::directory)
    {
        if(isExist(path, std::filesystem::file_type::directory))
            return true;
        else
            return std::filesystem::create_directories(path);
    }
    else
        return false;
}

void copy(const std::filesystem::path &from
    , const std::filesystem::path &to)
{
    if(std::filesystem::exists(to))
        std::filesystem::remove_all(to);
    
    std::filesystem::copy(from, to);
}

bool outFileError(const std::filesystem::path &path)
{
    std::cerr << "file error:\n"
        "    what: failed to operate file or directory.\n"
        "    path: " << path.string()
        << std::endl;
    return false;
}

}