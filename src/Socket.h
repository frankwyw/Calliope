#ifndef __SOCKET__H__
#define __SOCKET__H__

#include <memory>
#include <sys/socket.h>
#include <sys/types.h>

namespace honoka
{
    class Buffer;

    class Socket
    {
    public:
        Socket(int fd);
        Socket(int fd, struct sockaddr  servaddr, int len):fd_(fd), servaddr_(servaddr), len_(len);

        ~Socket();

        int accept();

        void set_servaddr(struct sockaddr  servaddr);
        struct sockaddr* get_servaddr_ptr();

        void set_len(int len);
        int get_len();

        inline int get_fd()
        {
            return fd_;
        }

        virtual int get_time();
        
        virtual void set_time();

    private:
        int fd_;

        struct sockaddr_in  servaddr_;
        int len_;

    };


    class Time_Socket : public Socket
    {
    public:
        Time_Socket(int fd, int time);
        Time_Socket(int fd, struct sockaddr  servaddr, int len, int time);
        int get_time();
        void set_time(int time);

    private:
        int time_;

    };
}

#endif
