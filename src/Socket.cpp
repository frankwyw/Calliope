#include "Socket.h"

#include <unistd.h>


namespace honoka
{
    Socket::Socket(int fd):fd_(fd), len(sizeof(struct sockaddr))
    {
        bzero(&servaddr, sizeof(struct sockaddr_in));
    }
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

   virtual int Socket::get_time()
   {
       return 0;
   }

   virtual void Socket::set_time()
   {

   }


    Time_Socket::Time_Socket(int fd, int time):Socket(fd), time_(time){}
    Time_Socket::Time_Socket(int fd, struct sockaddr  servaddr, int len, int time):Socket(fd, servaddr, len), time_(time){}

    int Time_Socket::get_time()
    {
        return time_;
    }
    void Time_Socket::set_time(int time)
    {
        time = time;
    }
}
