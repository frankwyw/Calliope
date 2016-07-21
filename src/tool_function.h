#ifndef __TOOL__FUNCTION__HPP__
#define __TOOL__FUNCTION__HPP__

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


namespace honoka
{
    void perror_and_exit(char * funname);

    int setnonblocking(int fd);
}
#endif
