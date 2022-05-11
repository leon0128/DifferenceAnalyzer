#ifndef PATH_HPP
#define PATH_HPP

#include <filesystem>
#include <string>

namespace PATH
{

/* type == file_type::redular
// if type is file_type::redular and path.parent() is not exist,
// path.parent() is created. if file is able to create in path,
// this function return true, otherwise function return false.
*/
/* type == file_type::directory
// if type is file_type::directory and path is not exist,
// path is created. if path was created or path is already created,
// function return true. otherwise function return false.
*/
/* otherwise
// function always return false.
*/
extern bool isValid(const std::filesystem::path &path
    , std::filesystem::file_type type = std::filesystem::file_type::regular);

inline extern bool isExist(const std::filesystem::path &path
    , std::filesystem::file_type type = std::filesystem::file_type::regular)
    {return type == std::filesystem::status(path).type();}

extern void copy(const std::filesystem::path &from
    , const std::filesystem::path &to);

extern bool outFileError(const std::filesystem::path&);

}

#endif