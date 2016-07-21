#include "Socket.h"

#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>


namespace honoka
{
    Socket::Socket(int fd):fd_(fd), len_(sizeof(struct ::sockaddr_in))
    {
	
    }

    Socket::Socket(int fd, std::shared_ptr<struct sockaddr_in> servaddr, socklen_t len):fd_(fd), servaddr_(servaddr), len_(len)
    {

    }


    Socket::~Socket(){ ::close(fd_);std::cout<<"~Socket"<<std::endl; }

    void Socket::set_servaddr(std::shared_ptr<struct sockaddr_in> servaddr)
    {
        servaddr_ = servaddr;
    }
    struct sockaddr_in* Socket::get_servaddr_ptr()
    {
        return servaddr_.get();
    }

    void Socket::set_len(int len)
    {
        len_ = len;
    }

    int Socket::get_len()
    {
        return len_;
    }

    int Socket::get_time()
    {
        return 0;
    }

    void Socket::set_time()
    {
	return;
    }


}
