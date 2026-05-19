#include "NonBlockingProcessLauncher.hpp"

#include <cerrno>
#include <system_error>

#include <unistd.h>

// 자식 종료를 추적하지 않음. zombie 는 호출자가 처리 (e.g. SIGCHLD=SIG_IGN).
ProcessResult NonBlockingProcessLauncher::launch(const std::string& path)
{
    pid_t pid = ::fork();
    if (pid < 0)
        throw std::system_error(errno, std::generic_category(), "fork");

    if (pid == 0)
    {
        ::execl(path.c_str(), path.c_str(), static_cast<char*>(nullptr));
        ::_exit(127);
    }

    return { 0, true };
}
