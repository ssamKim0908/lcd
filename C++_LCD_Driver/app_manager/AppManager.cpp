#include "AppManager.hpp"
#include "Manifest.hpp"

#include "interface/IProcessLauncher.hpp"
#include "sdk/Draw.hpp"
#include "shared/KeyEvent.hpp"
#include "util/color.hpp"
#include "util/Font.hpp"

#include <string>
#include <utility>

namespace
{

constexpr util::color::Rgb565 COLOR_BG        = util::color::BLACK;
constexpr util::color::Rgb565 COLOR_FG        = util::color::WHITE;
constexpr util::color::Rgb565 COLOR_HIGHLIGHT = util::color::YELLOW;
constexpr util::color::Rgb565 COLOR_TITLE     = util::color::CYAN;

constexpr int TITLE_X = 8;
constexpr int TITLE_Y = 8;
constexpr int LIST_X  = 24;
constexpr int LIST_Y  = 48;
constexpr int ROW_H   = 24;

}

AppManager::AppManager(std::vector<AppEntry> apps,
                       std::unique_ptr<IProcessLauncher> launcher)
    : apps_(std::move(apps)), launcher_(std::move(launcher))
{}

AppManager::~AppManager() = default;

void AppManager::on_render()
{
    draw().clear(COLOR_BG);
    draw().draw_text(TITLE_X, TITLE_Y, "App Manager",
                     util::font::TextSize::Small, COLOR_TITLE);

    int y = LIST_Y;
    for (std::size_t i = 0; i < apps_.size(); ++i)
    {
        const bool                selected = (i == selected_);
        const util::color::Rgb565 color    = selected ? COLOR_HIGHLIGHT : COLOR_FG;
        const std::string         line     = (selected ? "> " : "  ") + apps_[i].name;
        draw().draw_text(LIST_X, y, line, util::font::TextSize::Small, color);
        y += ROW_H;
    }
    draw().render();
}

void AppManager::on_key(const KeyEvent& ev)
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

void AppManager::launch(const AppEntry& app)
{
    launcher_->launch(app.path);
}
