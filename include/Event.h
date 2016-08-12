#ifndef __EVENT__H__
#define __EVENT__H__

#include <functional>

#include <memory>

#include "Event_Type.hpp"

//  0,passive connection
//  1,active connection
//  2,read callback
//  3,write callback
//  4,passive close
//  5,active close
//  6,timeout
namespace honoka
{
    struct Connection;
    class Reactor;
    class Connection_processor;

    class Event
    {
    public:
        //switch
        //PASSIV_CONN :reactor add_wait_fd,使用accepotr conn::passive_conn(conn_)
        //ACTIVE_CONN :reactor add_wait_fd，使用connector conn::active_conn(conn_)
        //READ_CB :conn_读取数据  conn::read(conn_);
        //WRITE_CB :conn_写入数据 conn::write(conn_);
        //PASSIVE_CLOSE :reactor 关闭conn conn::passive_close(conn_)
        //ACTIVE_CLOSE :reactor 关闭所有conn conn::passive_close(conn_)
        //TIMEOUT :reactor 关闭conn conn::timeout(conn_)

        void handle();

	    bool operator< (const Event& e2) const
	    {
	        return nice < e2.nice;
	    }

        Event(Reactor* reactor, Connection* conn, Event_Type type);
    private:
        std::function<void()> func_;
        Connection* conn_;
        Reactor* reactor_;
        Connection_processor* conn_processor_ ;
        Event_Type type_;
        int nice;

        static int type_to_nice[7];
    };



}




#endif
