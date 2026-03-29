#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/ioctl.h>

class ITransport_read
{
public:    
    virtual void trans_read() = 0;
};

class ITransport_write
{
public:    
    virtual void trans_write() = 0;
};

class ITransport_ioctl
{
public:    
    virtual void trans_ioctl() = 0;
};


class transport
{
private:
    int fd = -1;
public:
    transport(const std::string& device, int flags)
    {
        fd = open(device.c_str(), flags);
        if (fd < 0) {
            throw std::runtime_error("Failed to open device");
        }
    };

    transport(const transport&) = delete;
    transport& operator=(const transport&) = delete;

    transport(transport&&) = delete;
    transport& operator=(transport&&) = delete;

    virtual int get_fd() const = 0;

    virtual ~transport()
    {
        if (fd >= 0) 
        {
            close(fd);
        }
    }
};