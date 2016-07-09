#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


namespace honoka
{
    Socket::Socket(int fd):fd_(fd){}
    Socket::Socket(int fd, struct sockaddr  servaddr, int len):fd_(fd), servaddr_(servaddr), len_(len){}

    Socket::~Socket(){ ::close(fd_); }

    void set_servaddr(struct sockaddr  servaddr)
    {
        servaddr_ = servaddr;
    }
    struct sockaddr* get_servaddr_ptr()
    {
        return &servaddr_;
    }

    void set_len(int len)
    {
        len_ = len;
    }
    
    int get_len()
    {
        return len_;
    }

    int Socket::accept()
    {
        //(struct sockaddr *) &servaddr封装到socket里面吗？
        int tmp_fd = ::accpet(fd_);
        return tmp_fd;
    }

}
