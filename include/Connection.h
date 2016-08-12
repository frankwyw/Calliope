#ifndef __CONNECTION__H__
#define __CONNECTION__H__

#include <memory>
#include "Buffer.hpp"

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


    struct Connection
    {
    public:
        Connection(std::shared_ptr<Socket> socket_);
        std::shared_ptr<Socket> socket;


        std::shared_ptr<Ring_buffer<char>> buffer;

    //    Socket socket_;
    //    std::vector<std::shared_ptr<Event>> cb;

    };
}

#endif
