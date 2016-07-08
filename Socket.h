#ifndef __SOCKET__H__
#define __SOCKET__H__

#include <memory>
namespace honoka
{
    class Buffer;

    class Socket
    {
    public:
        Socket(int fd);
        ~Socket();

        void read();
        void write();

        int accept();

        inline int get_fd()
        {
            return fd_;
        }
    private:
        int fd_;
    };


    class Time_Socket : public Socket
    {
    public:
        int get_time();
        void set_time();

    private:
        int time;
    };
}

#endif
