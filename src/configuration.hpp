#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP

#include <vector>
#include <filesystem>

class Configuration
{
private:
    inline static const std::filesystem::path mScanningPathFile = "./scanning_path.json";
    inline static const char * mScanningPathsKey = "scanning_paths";
    inline static std::vector<std::filesystem::path> mScanningPaths = {};

public:
    Configuration() = delete;

    static bool initialize();
    
    static const std::vector<std::filesystem::path> &scanningPaths() noexcept
        {return mScanningPaths;}

private:
    static bool loadJson();
};

#endif