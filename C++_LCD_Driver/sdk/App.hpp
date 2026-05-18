#pragma once
#include "../shared/KeyEvent.hpp"
#include <memory>

namespace sdk
{

class Draw;
class Key;
class AppImpl;

class App
{
public:
    App();
    virtual ~App();

    App(const App&)            = delete;
    App& operator=(const App&) = delete;
    App(App&&)                 = delete;
    App& operator=(App&&)      = delete;

    void run();

protected:
    Draw& draw();
    Key&  key();
    void  exit();

    virtual void on_key   (const KeyEvent& ev) = 0;
    virtual void on_render()                   = 0;

private:
    bool                     running_ = false;
    std::unique_ptr<AppImpl> impl_;
};

}
