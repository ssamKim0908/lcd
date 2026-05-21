#include "AppManager.hpp"
#include "Manifest.hpp"
#include "FileReader/LocalFileReader.hpp"

#include "osal/process/BlockingProcessLauncher.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <csignal>

#ifndef APPS_CONF_PATH
#define APPS_CONF_PATH "apps.conf"
#endif

int main(int argc, char** argv)
{
    std::signal(SIGCHLD, SIG_DFL);

    const std::string manifest_path = (argc > 1) ? argv[1] : APPS_CONF_PATH;

    std::vector<AppEntry> apps;
    try
    {
        Manifest manifest(std::make_unique<LocalFileReader>());
        apps = manifest.load(manifest_path);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[app_manager] " << e.what() << std::endl;
        return 1;
    }

    auto launcher = std::make_unique<BlockingProcessLauncher>();
    AppManager mgr(std::move(apps), std::move(launcher));
    mgr.run();
    return 0;
}
