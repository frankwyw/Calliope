#ifndef __TOOL__FUNCTION__HPP__
#define __TOOL__FUNCTION__HPP__

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


namespace honoka
{
    void perror_and_exit(char * funname)
    {
        perror(funname);
        exit(EXIT_FAILURE);
    }

    int setnonblocking(int fd)
    {
        if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1)
        {
            LOG(ERROR)<<"setnonblocking() fail";
            return -1;
        }
        return 0;
    }
}
#endif
