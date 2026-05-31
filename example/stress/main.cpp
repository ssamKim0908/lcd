#include <sdk/App.hpp>
#include <sdk/Draw.hpp>
#include <util/color.hpp>

#include <cstdint>
#include <cstdlib>
#include <fstream>

namespace
{
constexpr int  DEFAULT_N = 1000;
constexpr char N_FILE[]  = "/tmp/stress_n";

// 한 프레임에 호출할 draw call 수.
// 우선순위: 환경변수 STRESS_N  →  파일 /tmp/stress_n  →  기본값.
// app_manager 는 execl(path, path, nullptr) 로 fork+exec 하므로 인자/실행시점 env 를
// 줄 수 없다 → 파일 경로가 주력. on_draw 마다 다시 읽으므로, 파일만 바꾸고 아무 키나
// 누르면(App::run 이 매 키마다 on_draw 재호출) 재실행 없이 N 을 바꿔 스윕할 수 있다.
int draws_per_frame()
{
    if (const char* s = std::getenv("STRESS_N"))
    {
        int n = std::atoi(s);
        if (n > 0) return n;
    }
    if (std::ifstream f{ N_FILE })
    {
        int n = 0;
        if (f >> n && n > 0) return n;
    }
    return DEFAULT_N;
}

// 가운데 사각형 (Center 키로 색 순환). 키 반응시간 눈으로 확인용 마커.
constexpr int SCREEN = 240;
constexpr int RECT_W = 40;
constexpr int RECT_H = 40;
constexpr int RECT_X = (SCREEN - RECT_W) / 2;
constexpr int RECT_Y = (SCREEN - RECT_H) / 2;

constexpr util::color::Rgb565 PALETTE[] = {
    util::color::RED,
    util::color::GREEN,
    util::color::BLUE,
    util::color::YELLOW,
    util::color::CYAN,
    util::color::MAGENTA,
    util::color::WHITE,
};
constexpr int PALETTE_N = sizeof(PALETTE) / sizeof(PALETTE[0]);
}

// 측정용 부하 생성 앱.
// App::run() 이 시작 시 on_draw()+on_render() 를 1회 자동 실행하므로 실행 즉시 첫 burst 가
// 나가고, 이후 K4 외의 키를 누를 때마다 N 을 다시 읽어 새 burst 를 낸다.
// Center 키를 누르면 가운데 사각형 색이 바뀐다 → 키가 제때 반영되는지 눈으로 확인.
class StressApp : public sdk::App
{
protected:
    void on_draw() override
    {
        const int n = draws_per_frame();
        for (int i = 0; i < n; ++i)
            draw().fill_rect(0, 0, 1, 1, util::color::WHITE);  // 최소 페이로드 draw (기존 부하)

        // 가운데 마커 사각형 (현재 색으로 매 프레임 덮어 그림)
        draw().fill_rect(RECT_X, RECT_Y, RECT_W, RECT_H, PALETTE[color_idx_]);
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state != KeyState::Pressed) return;

        if (ev.key == Key::K4)     { App::exit(); return; }
        if (ev.key == Key::Center) color_idx_ = (color_idx_ + 1) % PALETTE_N;
        // 그 외 키는 no-op → App::run 이 이어서 on_draw() 를 다시 호출(새 burst)
    }

private:
    int color_idx_ = 0;
};

int main()
{
    StressApp app;
    app.run();
    return 0;
}
