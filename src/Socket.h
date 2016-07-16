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
        Socket(int fd, std::shared_ptr<struct sockaddr_in> servaddr, socklen_t len);

        ~Socket();


        void set_servaddr(std::shared_ptr<struct sockaddr_in> servaddr);
        struct sockaddr_in* get_servaddr_ptr();

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

        std::shared_ptr<struct sockaddr_in>  servaddr_;
        socklen_t len_;

    };


}

#endif
