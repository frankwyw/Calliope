#ifndef __CONNECTION__H__
#define __CONNECTION__H__

#include <vector>
#include "Buffer.h"

//  0,passive connection
//  1,active connection
//  2,read callback
//  3,write callback
//  4,passive close
//  5,active close
//  6,timeout
//  vector中的6个事件回调，默认指向一个初始化表（共享模式？！）
namespace honoka
{
    class Socket;
    class Event;
    class Buffer;
    class Reactor;

    class Connection
    {
    public:

        std::shared_ptr<Socket> get_socket()
        {
            return socket_;
        }

        Buffer& get_buffer()
        {
            return buffer_;
        }

        Connection(Reactor* reactor, std::shared_ptr<Socket> socket);
    private:
        std::shared_ptr<Socket> socket_;
        Buffer buffer_;
        Reactor* reactor_;
    //    Socket socket_;
    //    std::vector<std::shared_ptr<Event>> cb;

    };
}

#endif
