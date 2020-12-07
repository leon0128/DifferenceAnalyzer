#ifndef CONFIGURE_HPP
#define CONFIGURE_HPP

#include <vector>
#include <filesystem>

class Configuration
{
private:
    inline static const std::filesystem::path mConfigurationPath = "./configuration.json";
    inline static const char *mOutputDirKey = "output_dir";
    inline static std::filesystem::path mOutputDir = std::filesystem::path();
    inline static const char * mScanningPathsKey = "scanning_paths";
    inline static std::vector<std::filesystem::path> mScanningPaths = {};

public:
    Configuration() = delete;

    static bool initialize();
    
    static const std::filesystem::path &outputDir() noexcept
        {return mOutputDir;}
    static const std::vector<std::filesystem::path> &scanningPaths() noexcept
        {return mScanningPaths;}

private:
    static bool loadJson();
};

#endif