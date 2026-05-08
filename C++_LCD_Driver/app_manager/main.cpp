#include "Manifest.hpp"
#include "FileReader/LocalFileReader.hpp"

#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

#ifndef APPS_CONF_PATH
#define APPS_CONF_PATH "apps.conf"
#endif

namespace
{

constexpr uint16_t COLOR_BG        = 0x0000;
constexpr uint16_t COLOR_FG        = 0xFFFF;
constexpr uint16_t COLOR_HIGHLIGHT = 0xFFE0;
constexpr uint16_t COLOR_TITLE     = 0x07FF;

constexpr int32_t TITLE_X = 8;
constexpr int32_t TITLE_Y = 8;
constexpr int32_t LIST_X  = 24;
constexpr int32_t LIST_Y  = 48;
constexpr int32_t ROW_H   = 24;

}

class AppManager : public sdk::App
{
public:
    explicit AppManager(std::vector<AppEntry> apps)
        : apps_(std::move(apps)) {}

protected:
    void on_render() override
    {
        draw().clear(COLOR_BG);
        draw().draw_text(TITLE_X, TITLE_Y, util::font::TextSize::Small,
                         COLOR_TITLE, "App Manager");

        int32_t y = LIST_Y;
        for (std::size_t i = 0; i < apps_.size(); ++i)
        {
            const bool     selected = (i == selected_);
            const uint16_t color    = selected ? COLOR_HIGHLIGHT : COLOR_FG;
            const std::string line  = (selected ? "> " : "  ") + apps_[i].name;
            draw().draw_text(LIST_X, y, util::font::TextSize::Small, color, line);
            y += ROW_H;
        }
        draw().render();
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state != KeyState::Pressed) return;
        if (apps_.empty()) return;

        switch (ev.key)
        {
        case Key::Up:
            if (selected_ > 0) --selected_;
            break;
        case Key::Down:
            if (selected_ + 1 < apps_.size()) ++selected_;
            break;
        case Key::Center:
            launch(apps_[selected_]);
            break;
        default:
            break;
        }
    }

private:
    void launch(const AppEntry& app)
    {
        pid_t pid = ::fork();
        if (pid < 0) return;
        if (pid == 0)
        {
            ::execl(app.path.c_str(), app.path.c_str(), static_cast<char*>(nullptr));
            ::_exit(127);
        }
        int status = 0;
        ::waitpid(pid, &status, 0);
    }

    std::vector<AppEntry> apps_;
    std::size_t           selected_ = 0;
};

int main(int argc, char** argv)
{
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

    AppManager mgr(std::move(apps));
    mgr.run();
    return 0;
}
