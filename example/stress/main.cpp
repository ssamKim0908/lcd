#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

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
}

// 측정용 부하 생성 앱.
// App::run() 이 시작 시 on_draw()+on_render() 를 1회 자동 실행하므로 실행 즉시 첫 burst 가
// 나가고, 이후 K4 외의 키를 누를 때마다 N 을 다시 읽어 새 burst 를 낸다.
class StressApp : public sdk::App
{
protected:
    void on_draw() override
    {
        const int n = draws_per_frame();
        for (int i = 0; i < n; ++i)
            draw().fill_rect(0, 0, 1, 1, 0xFFFF);  // 최소 페이로드 draw
    }

    void on_key(const KeyEvent& ev) override
    {
        if (ev.state == KeyState::Pressed && ev.key == Key::K4)
            App::exit();
        // 그 외 키는 no-op → App::run 이 이어서 on_draw() 를 다시 호출(새 burst)
    }
};

int main()
{
    StressApp app;
    app.run();
    return 0;
}
