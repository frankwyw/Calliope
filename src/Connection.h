#ifndef __CONNECTION__H__
#define __CONNECTION__H__

#include <memory>

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
    class Buffer;

    class Connection
    {
    public:

        std::shared_ptr<Socket> get_socket();

        std::shared_ptr<Buffer> get_buffer();

        void set_buffer set_buffer(std::shared_ptr<Buffer> buffer);

        Connection(Reactor* reactor, std::shared_ptr<Socket> socket);
    private:
        std::shared_ptr<Socket> socket_;
        std::shared_ptr<Buffer> buffer_;
        Reactor* reactor_;

    //    Socket socket_;
    //    std::vector<std::shared_ptr<Event>> cb;

    };
}

#endif
