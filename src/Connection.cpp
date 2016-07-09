#include "Connection.h"

namespace honoka
{

    Connection(Reactor* reactor, std::shared_ptr<Socket> socket):reactor_(reactor), socket_(socket)
    {
        cb_[0] = &passive_conn;
        cb_[1] = &active_conn;
        cb_[2] = &read;
        cb_[3] = &write;
        cb_[4] = &passive_close;
        cb_[5] = &active_close;
        cb_[6] = &timeout;
    }

    void set_cb(int i, std::function<void(Connection* )> cb)
    {
        cb_[i] = cb;
    }

    std::shared_ptr<Socket> Connection::get_socket()
    {
        return socket_;
    }

    Buffer&  Connection::get_buffer()
    {
        return buffer_;
    }

    void set_buffer set_buffer(std::shared_ptr<Buffer> buffer)
    {
        buffer_ = buffer
    }
}
