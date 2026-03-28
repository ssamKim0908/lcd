#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/ioctl.h>

class transport 
{
private:
    int fd;
public:
    transport(const std::string& device)
    {
        fd = open(device.c_str(), O_RDWR);
        if (fd < 0) {
            throw std::runtime_error("Failed to open device");
        }
    };

    transport(const transport&) = delete;
    transport& operator=(const transport&) = delete;

    transport(transport&&) = default;
    transport& operator=(transport&&) = default;

    virtual void trans_read() = 0;
    virtual void trans_write() = 0;
    virtual void trans_ioctl() = 0;
    virtual bool trans_init() = 0;

    int get_fd() const { return fd; }

    ~transport()
    {
        if (fd >= 0) 
        {
            close(fd);
        }
    }
};