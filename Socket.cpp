#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


namespace honoka
{
Socket::Socket(int fd):fd_(fd){}

Socket::~Socket(){ ::close(fd_); }

void Socket::read()
{


}

void Socket::write()
{

}

int Socket::accept()
{
    //(struct sockaddr *) &servaddr封装到socket里面吗？
    int tmp_fd = ::accpet(fd_);
    return tmp_fd;
}

}
