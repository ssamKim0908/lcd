#include "LocalFileReader.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>

#include <dirent.h>

std::vector<std::string> LocalFileReader::read_lines(const std::string& path)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("LocalFileReader: cannot open " + path);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(std::move(line));
    return lines;
}

std::vector<std::string> LocalFileReader::list_files(const std::string& dir,
                                                     const std::string& ext)
{
    std::vector<std::string> result;

    DIR* d = ::opendir(dir.c_str());
    if (!d) return result;

    while (auto* ent = ::readdir(d))
    {
        const std::string name = ent->d_name;
        if (name == "." || name == "..") continue;

        if (!ext.empty())
        {
            if (name.size() < ext.size()) continue;
            if (name.compare(name.size() - ext.size(), ext.size(), ext) != 0)
                continue;
        }
        result.push_back(dir + "/" + name);
    }
    ::closedir(d);

    std::sort(result.begin(), result.end());
    return result;
}
