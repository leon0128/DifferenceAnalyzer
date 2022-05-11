#include <exception>
#include <iostream>
#include <regex>

#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "path.hpp"
#include "configuration.hpp"
#include "analyzer.hpp"

Analyzer::Analyzer(const std::filesystem::path &path)
    : mPath(path)
{
}

bool Analyzer::analyze() const
{
    if(!PATH::isExist(mPath))
        return PATH::outFileError(mPath);

    boost::property_tree::ptree ptree;
    try
        {boost::property_tree::read_json(mPath, ptree);}
    catch(const std::exception &e)
    {
        std::cerr << "Analyzer::loadJson() error:\n"
            "    what: " << e.what() << "\n"
            "    path: " << mPath.string()
            << std::endl;
        return false;
    }

    bool isOutput = false;
    std::vector<std::string> subLineVec, addLineVec;
    // difference.
    if(auto &&optDiffArr = ptree.get_child_optional(DIFFERENCE_KEY); optDiffArr)
    {
        for(auto &&diff : optDiffArr.get())
        {
            // difference.hunk.
            if(auto &&optHunkArr = diff.second.get_child_optional(HUNK_KEY); optHunkArr)
            {
                for(auto &&hunk : optHunkArr.get())
                {
                    // difference.hunk.sub.
                    if(auto &&optSubArr = hunk.second.get_child_optional(SUB_KEY); optSubArr)
                    {
                        for(auto &&sub : optSubArr.get())
                        {
                            if(auto &&optLine = sub.second.get_optional<std::string>(LINE_KEY); optLine)
                                subLineVec.emplace_back(optLine.get());
                        }
                    }
                    // difference.hunk.add.
                    if(auto &&optAddArr = hunk.second.get_child_optional(ADD_KEY); optAddArr)
                    {
                        for(auto &&add : optAddArr.get())
                        {
                            if(auto &&optLine = add.second.get_optional<std::string>(LINE_KEY); optLine)
                                addLineVec.emplace_back(optLine.get());
                        }
                    }

                    if(isMatch(subLineVec, addLineVec))
                        isOutput = true;
                    
                    subLineVec.clear();
                    addLineVec.clear();
                }
            }
        }
    }

    if(isOutput)
        output();

    return true;
}

bool Analyzer::isMatch(const std::vector<std::string> &sub
    , const std::vector<std::string> &add) const
{
    bool ret = false;

    if(isChangedAnyLine(sub, add, 100, 1) && (ret = true))
        std::cout << "match: isChangedAnyLine\n";
    // if(isAddingNullChecker(sub, add) && (ret = true))
    //     std::cout << "match: isAddingNullChecker\n";

    std::cout << std::flush;
    return ret;
}

bool Analyzer::isChangedAnyLine(const std::vector<std::string> &sub
    , const std::vector<std::string> &add
    , std::size_t subMax
    , std::size_t addMax) const
{
    if(sub.size() <= subMax && add.size() <= addMax)
        return true;
    else
        return false;
}

bool Analyzer::isAddingNullChecker(const std::vector<std::string> &sub
    , const std::vector<std::string> &add) const
{
    // c, c++, java
    // if(!?var)
    static const std::regex ifRe(std::string(WS_RE)
        + "if" + WS_RE
        + "\\(" + WS_RE
        + "!?" + WS_RE
        + VAR_RE + WS_RE
        + "\\)" + ANY_RE);
    // if(var (==|!=) (null|NULL|nullptr))
    static const std::regex ifCompRe(std::string(WS_RE)
        + "if" + WS_RE
        + "\\(" + WS_RE
        + VAR_RE + WS_RE
        + "(==|!=)" + WS_RE
        + "(null|NULL|nullptr)" + WS_RE
        + "\\)" + ANY_RE);
    // python
    // if (not)? var:
    static const std::regex pyIfRe(std::string(WS_RE)
        + "if" + WS_RE
        + "(not)?" + WS_RE
        + VAR_RE + WS_RE
        + ":" + ANY_RE);
    // if var is (not)? None:
    static const std::regex pyIfCompRe(std::string(WS_RE)
        + "if" + WS_RE
        + VAR_RE + WS_RE
        + "is" + WS_RE
        + "(not)?" + WS_RE
        + "None" + WS_RE
        + ":" + WS_RE);
    
    bool ret = false;
    for(auto &&s : add)
    {
        if((std::regex_match(s, ifRe) || std::regex_match(s, ifCompRe)
                || std::regex_match(s, pyIfRe) || std::regex_match(s, pyIfCompRe))
            && (ret = true))
            std::cout << "line: " << s << std::endl;
    }

    return ret;
}

void Analyzer::output() const
{
    std::filesystem::path toPath(Configuration::outputDir() / mPath.filename());
    PATH::copy(mPath, toPath);
    std::cout << "output:\n"
        "    from: " << mPath.string() << "\n"
        "    to: " << toPath.string()
        << std::endl;
}