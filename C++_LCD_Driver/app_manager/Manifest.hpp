#pragma once
#include <memory>
#include <string>
#include <vector>

class IFileReader;

struct AppEntry
{
    std::string name;
    std::string path;
};

class Manifest
{
public:
    explicit Manifest(std::unique_ptr<IFileReader> reader);
    ~Manifest();

    Manifest(const Manifest&)            = delete;
    Manifest& operator=(const Manifest&) = delete;

    std::vector<AppEntry> load(const std::string& path);

private:
    std::unique_ptr<IFileReader> reader_;
};
