
#include "Buffer.h"
#include <memory>


class Socket
{
public:
    Socket(int fd);
    ~Socket();

    void read(std::shared_ptr<Buffer> buffer);
    void write(std::shared_ptr<Buufer> buffer);

    inline int get_fd()
    {
        return fd_;
    }
private:
    int fd_;
}


class Time_Socket : public Socket
{
public:
    int get_time();
    void set_time();

private:
    int time;
}
