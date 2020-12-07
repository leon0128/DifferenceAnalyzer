#include "path.hpp"

#include <filesystem>

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

}