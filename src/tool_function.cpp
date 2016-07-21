#include "tool_function.h"
#include <glog/logging.h>

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
