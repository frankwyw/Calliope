#include "Connection_processor.h"

#include <memory>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>

#include <glog/logging.h>

#include "Event.h"
#include "Reactor.h"
#include "Connection.h"
#include "Socket.h"
#include "Buffer.hpp"



namespace honoka
{
    Connection_processor::Connection_processor()
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

    void Connection_processor::handle(Connection* conn, Event_Type type, Reactor* reactor_)
    {
        DLOG(INFO)<<"Connection_processor::handle() fd:"<<conn->socket->get_fd();
        //std::cout<<"please define processor handle()"<<std::endl;
        //用户定义std::bind(func_, _1, _2);
        cb_[static_cast<int>(type)](this, conn);
        auto tmp_socket = conn->socket;
        switch (type) {
            case PASSIV_CONN:
            case ACTIVE_CONN:
                //reactor_->add_wait(tmp_socket);
                //if(conn->buffer == nullptr)
                //    ;//conn->buffer = std::make_shared<Ring_buffer<char>>();
                break;
            case READ_CB:
            case WRITE_CB:
                break;
            case PASSIVE_CLOSE:
            case ACTIVE_CLOSE:
                //reactor_->del_listen(tmp_socket);
                break;
            case TIMEOUT:
                reactor_->del_wait(tmp_socket->get_fd());
                break;
            default:
                break;
        }
    }

    //自定义链接为一个conn绑定一个buffer，或者默认指定为1000；
    void Connection_processor::passive_conn(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::passive_conn(Connection*)";
//	    Ring_buffer<char> aaaa(1000);
        conn->buffer = std::make_shared<Ring_buffer<char>>(1000);
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
	    char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"passive new conn connect, ip:"<<addr;
    }
    void Connection_processor::active_conn(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::active_conn(Connection*)";
//        conn->buffer = std::make_shared<Ring_buffer<char>>();
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
	    char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"active new connect, ip:"<<addr;
    }
    void Connection_processor::read(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::read(Connection*)";
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"read , ip:"<<addr;
    }
    void Connection_processor::write(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::write(Connection*)";
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"write , ip:"<<addr;
    }
    void Connection_processor::passive_close(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::passive_close(Connection*)";
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"passive close connect, ip:"<<addr;
    }
    void Connection_processor::active_close(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::active_close(Connection*)";
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"active close connect, ip:"<<addr;
    }
    void Connection_processor::timeout(Connection* conn)
    {
        DLOG(INFO)<<"Connection_processor::timeout(Connection*)";
        auto tmp_socket = conn->socket;
        auto servaddr = tmp_socket->get_servaddr_ptr();
        char* addr = inet_ntoa(servaddr->sin_addr);
        LOG(INFO)<<"close timeout, ip:"<<addr;
    }

    void Connection_processor::set_cb(std::function<void (Connection_processor*, Connection*)> cb, int i)
    {
        DLOG(INFO)<<"Connection_processor::set_cb(Connection*)";
        cb_[i] = cb;
    }



}
