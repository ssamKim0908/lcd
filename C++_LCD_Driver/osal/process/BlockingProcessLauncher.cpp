#include "BlockingProcessLauncher.hpp"

#include <cerrno>
#include <system_error>

#include <sys/wait.h>
#include <unistd.h>

ProcessResult BlockingProcessLauncher::launch(const std::string& path)
{
    pid_t pid = ::fork();
    if (pid < 0)
        throw std::system_error(errno, std::generic_category(), "fork");

    if (pid == 0)
    {
        ::execl(path.c_str(), path.c_str(), static_cast<char*>(nullptr));
        ::_exit(127);
    }

    int status = 0;
    if (::waitpid(pid, &status, 0) < 0)
        throw std::system_error(errno, std::generic_category(), "waitpid");

    ProcessResult r;
    r.exited_normally = WIFEXITED(status);
    r.exit_code       = r.exited_normally ? WEXITSTATUS(status) : -1;
    return r;
}
