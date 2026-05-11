#pragma once
#include "KeyEvent.hpp"
#include <memory>

namespace sdk
{

class Draw; //header file 전방 선언

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
    void  exit();

    virtual void on_key   (const KeyEvent& ev) = 0;
    virtual void on_render()                   = 0;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
