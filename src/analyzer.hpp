#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <filesystem>
#include <vector>
#include <string>

class Analyzer
{
private:
    // json
    inline static const char *DIFFERENCE_KEY = "difference";
    inline static const char *HUNK_KEY = "hunk";
    inline static const char *SUB_KEY = "sub";
    inline static const char *ADD_KEY = "add";
    inline static const char *LINE_KEY = "";

    // regex
    inline static const char *WS_RE = R"del([ \t\f\r\n]*)del";
    inline static const char *VAR_RE = R"del([a-zA-Z_][a-zA-Z_0-9]*)del";
    inline static const char *ANY_RE = R"del(.*)del";

public:
    Analyzer(const std::filesystem::path &path);

    bool analyze() const;

private:
    bool isMatch(const std::vector<std::string> &sub
        , const std::vector<std::string> &add) const;
    bool isChangedAnyLine(const std::vector<std::string> &sub
        , const std::vector<std::string> &add
        , std::size_t subMax
        , std::size_t addMax) const;
    bool isAddingNullChecker(const std::vector<std::string> &sub
        , const std::vector<std::string> &add) const;
    
    void output() const;

    std::filesystem::path mPath;
};

#endif