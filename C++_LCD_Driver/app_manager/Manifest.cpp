#include "Manifest.hpp"

#include "interface/IFileReader.hpp"

#include <stdexcept>

namespace
{

constexpr char SEPARATOR = '|';
constexpr char COMMENT   = '#';

std::string trim(const std::string& s)
{
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end   = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

}

Manifest::Manifest(std::unique_ptr<IFileReader> reader)
    : reader_(std::move(reader))
{}

Manifest::~Manifest() = default;

std::vector<AppEntry> Manifest::load(const std::string& path)
{
    std::vector<std::string> lines = reader_->read_lines(path);
    std::vector<AppEntry>    apps;

    for (std::size_t i = 0; i < lines.size(); ++i)
    {
        const std::string trimmed = trim(lines[i]);
        if (trimmed.empty() || trimmed.front() == COMMENT) continue;

        const auto sep = trimmed.find(SEPARATOR);
        if (sep == std::string::npos)
            throw std::runtime_error("Manifest: missing '|' on line "
                                     + std::to_string(i + 1));

        AppEntry entry;
        entry.name = trim(trimmed.substr(0, sep));
        entry.path = trim(trimmed.substr(sep + 1));

        if (entry.name.empty() || entry.path.empty())
            throw std::runtime_error("Manifest: empty name or path on line "
                                     + std::to_string(i + 1));

        apps.push_back(std::move(entry));
    }

    return apps;
}
