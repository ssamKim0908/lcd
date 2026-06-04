#include "FakeKeys.hpp"

#include <sys/timerfd.h>
#include <unistd.h>
#include <cstdint>
#include <cerrno>
#include <stdexcept>
#include <system_error>

FakeKeys::FakeKeys(int period_ms)
{
    if (period_ms <= 0)
        throw std::invalid_argument("FakeKeys: period_ms must be > 0");

    fd_ = ::timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd_ < 0)
        throw std::system_error(errno, std::generic_category(), "timerfd_create");

    itimerspec ts{};
    ts.it_value.tv_sec  = period_ms / 1000;
    ts.it_value.tv_nsec = (period_ms % 1000) * 1'000'000L;
    ts.it_interval      = ts.it_value;            // 주기 반복

    if (::timerfd_settime(fd_, 0, &ts, nullptr) < 0)
        throw std::system_error(errno, std::generic_category(), "timerfd_settime");
}

FakeKeys::~FakeKeys()
{
    if (fd_ >= 0) ::close(fd_);
}

int FakeKeys::fd() const
{
    return fd_;
}

KeyEvent FakeKeys::next_event()
{
    // timerfd 만료 소비(읽지 않으면 계속 readable). expirations 는 누락된 틱 수.
    uint64_t expirations = 0;
    if (::read(fd_, &expirations, sizeof(expirations)) < 0)
        throw std::system_error(errno, std::generic_category(), "FakeKeys read timerfd");

    down_ = !down_;
    return { Key::Center, down_ ? KeyState::Pressed : KeyState::Released };
}
