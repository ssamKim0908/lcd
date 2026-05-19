#pragma once
#include "sdk/App.hpp"

#include <cstddef>
#include <memory>
#include <vector>

struct AppEntry;
class IProcessLauncher;

class AppManager : public sdk::App
{
public:
    AppManager(std::vector<AppEntry> apps, std::unique_ptr<IProcessLauncher> launcher);
    ~AppManager() override;

protected:
    void on_render() override;
    void on_key(const KeyEvent& ev) override;

private:
    void launch(const AppEntry& app);

    std::vector<AppEntry>             apps_;
    std::unique_ptr<IProcessLauncher> launcher_;
    std::size_t                       selected_ = 0;
};
