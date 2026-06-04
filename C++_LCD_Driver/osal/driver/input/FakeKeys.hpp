#pragma once
#include "../../../interface/IKeys.hpp"

//key 입력 test용 class
class FakeKeys : public IKeys
{
private:
    int  fd_   = -1;     // timerfd
    bool down_ = false;  // 다음 이벤트 상태(press/release 교대)
public:
    explicit FakeKeys(int period_ms);
    ~FakeKeys() override;

    FakeKeys(const FakeKeys&)            = delete;
    FakeKeys& operator=(const FakeKeys&) = delete;

    int      fd        () const override;
    KeyEvent next_event()       override;
};
