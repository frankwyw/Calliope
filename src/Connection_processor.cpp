#include "Connection_processor.h"

#include <memory>

#include "Event.h"
#include "Reactor.h"
#include "Connection.h"
#include "Socket.h"
#include "Buffer.h"



namespace honoka
{
    Connection_processor::Connection_processor(Reactor* reactor):reactor_(reactor)
    {
        cb_[0] = &passive_conn;
        cb_[1] = &active_conn;
        cb_[2] = &read;
        cb_[3] = &write;
        cb_[4] = &passive_close;
        cb_[5] = &active_close;
        cb_[6] = &timeout;
    }

    Connection_processor::~Connection_processor(){}

    void Connection_processor::handle(std::shared_ptr<Connection> conn, Event_Type type)
    {
        //用户定义std::bind(func_, _1, _2);
        cb_[static_cast<int>(type)](this, conn);
        switch (type) {
            case PASSIV_CONN:
            case ACTIVE_CONN:
                auto tmp_socket = conn->socket;
                reactor_->add_wait(tmp_socket);
                if(conn->buffer == nullptr)
                    conn->set_buffer(Buffer(4000));
                break;
            case READ_CB:
            case WRITE_CB:
                break;
            case PASSIVE_CLOSE:
            case ACTIVE_CLOSE:
                auto tmp_socket = conn->socket;
                reactor_->del_listen(tmp_socket);
                break;
            case TIMEOUT:
                auto tmp_socket = conn->socket;
                reactor_->del_wait(tmp_socket);
                break;
            default:
                break;
        }
    }

    //自定义链接为一个conn绑定一个buffer，或者默认指定为4000；
    void Connection_processor::passive_conn(std::shared_ptr<Connection> conn)
    {
        conn->buffer = std::move(std::make_shared<Buffer>(4000));
    }
    void Connection_processor::active_conn(std::shared_ptr<Connection> conn)
    {
        conn->buffer = std::move(std::make_shared<Buffer>(4000));
    }
    void Connection_processor::read(std::shared_ptr<Connection> conn)
    {
        
    }
    void Connection_processor::write(std::shared_ptr<Connection> conn)
    {

    }
    void Connection_processor::passive_close(std::shared_ptr<Connection> conn)
    {

    }
    void Connection_processor::active_close(std::shared_ptr<Connection> conn)
    {

    }
    void Connection_processor::timeout(std::shared_ptr<Connection> conn)
    {

    }

    void set_cb(std::function<void (Connection_processor*, std::shared_ptr<Connection>)> cb, int i)
    {
        cb_[i] = cb;
    }



}
