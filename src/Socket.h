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

        int accept();

        void set_servaddr(struct sockaddr  servaddr);
        struct sockaddr* get_servaddr_ptr();

        void set_len(int len);
        int get_len();

        inline int get_fd()
        {
            return fd_;
        }
    private:
        int fd_;

        struct sockaddr  servaddr_;
        int len_;

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
