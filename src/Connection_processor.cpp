#include "Connection_processor.h"

#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>

#include <glog/logging.h>

#include "Event.h"
#include "Reactor.h"
#include "Connection.h"
#include "Socket.h"
#include "Buffer.h"



namespace honoka
{
    Connection_processor::Connection_processor(Reactor* reactor):reactor_(reactor)
    {
	using std::placeholders::_1;
	using std::placeholders::_2;
        cb_[0] = std::bind(&Connection_processor::passive_conn, _1, _2);
        cb_[1] = std::bind(&Connection_processor::active_conn, _1, _2);
        cb_[2] = std::bind(&Connection_processor::read, _1, _2);
        cb_[3] = std::bind(&Connection_processor::write, _1, _2);
        cb_[4] = std::bind(&Connection_processor::passive_close, _1, _2);
        cb_[5] = std::bind(&Connection_processor::active_close, _1, _2);
        cb_[6] = std::bind(&Connection_processor::timeout, _1, _2);
    }

    Connection_processor::~Connection_processor(){}

    void Connection_processor::handle(std::shared_ptr<Connection> conn, Event_Type type)
    {
        //用户定义std::bind(func_, _1, _2);
        cb_[static_cast<int>(type)](this, conn);
        auto tmp_socket = conn->socket;
        switch (type) {
            case PASSIV_CONN:
            case ACTIVE_CONN:
                reactor_->add_wait(tmp_socket);
                if(conn->buffer == nullptr)
                    conn->buffer = std::make_shared<Ring_buffer<char>>();
                break;
            case READ_CB:
            case WRITE_CB:
                break;
            case PASSIVE_CLOSE:
            case ACTIVE_CLOSE:
                reactor_->del_listen(tmp_socket);
                break;
            case TIMEOUT:
                reactor_->del_wait(tmp_socket);
                break;
            default:
                break;
        }
    }

    //自定义链接为一个conn绑定一个buffer，或者默认指定为4000；
    void Connection_processor::passive_conn(std::shared_ptr<Connection> conn)
    {
        conn->buffer = std::make_shared<Ring_buffer<char>>();
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"passive new conn connect, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::active_conn(std::shared_ptr<Connection> conn)
    {
        conn->buffer = std::make_shared<Ring_buffer<char>>();
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"active new connect, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::read(std::shared_ptr<Connection> conn)
    {
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"read , ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::write(std::shared_ptr<Connection> conn)
    {
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"write , ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::passive_close(std::shared_ptr<Connection> conn)
    {
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"passive close connect, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::active_close(std::shared_ptr<Connection> conn)
    {
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"active close connect, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }
    void Connection_processor::timeout(std::shared_ptr<Connection> conn)
    {
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        LOG(INFO)<<"close timeout, ip:"<<servaddr->sin_addr.s_addr<<" port:"<<servaddr->sin_port;
    }

    void Connection_processor::set_cb(std::function<void (Connection_processor*, std::shared_ptr<Connection>)> cb, int i)
    {
        cb_[i] = cb;
    }



}
