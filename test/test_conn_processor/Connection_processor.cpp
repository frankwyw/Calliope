#include "Connection_processor.h"

#include <memory>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>




namespace honoka
{
		using std::placeholders::_1;
	using std::placeholders::_2;
    Connection_processor::Connection_processor()
    {

        cb_[0] = std::bind(&Connection_processor::passive_conn, _1);
        cb_[1] = std::bind(&Connection_processor::active_conn, _1);
        cb_[2] = std::bind(&Connection_processor::read, _1);
        cb_[3] = std::bind(&Connection_processor::write, _1);
        cb_[4] = std::bind(&Connection_processor::passive_close, _1);
        cb_[5] = std::bind(&Connection_processor::active_close, _1);
        cb_[6] = std::bind(&Connection_processor::timeout, _1);
    }

    Connection_processor::~Connection_processor(){}

    void Connection_processor::handle(Event_Type type)
    {
//	std::cout<<"Connection_processor::handle() fd:"<<conn->socket->get_fd();
	std::cout<<"please define processor handle()"<<std::endl;
        //用户定义std::bind(func_, _1, _2);
        cb_[static_cast<int>(type)](this);
    }

    //自定义链接为一个conn绑定一个buffer，或者默认指定为4000；
    void Connection_processor::passive_conn()
    {
	std::cout<<"Connection_processor::passive_conn(std::shared_ptr<Connection>)";
    }
    void Connection_processor::active_conn()
    {
	std::cout<<"Connection_processor::active_conn(std::shared_ptr<Connection>)";
    }
    void Connection_processor::read()
    {
	std::cout<<"Connection_processor::read(std::shared_ptr<Connection>)";
    }
    void Connection_processor::write()
    {
	std::cout<<"Connection_processor::write(std::shared_ptr<Connection>)";
    }
    void Connection_processor::passive_close()
    {
	std::cout<<"Connection_processor::passive_close(std::shared_ptr<Connection>)";
    }
    void Connection_processor::active_close()
    {
	std::cout<<"Connection_processor::active_close(std::shared_ptr<Connection>)";
    }
    void Connection_processor::timeout()
    {
	std::cout<<"Connection_processor::timeout(std::shared_ptr<Connection>)";
    }

//    void Connection_processor::set_cb(std::function<void (Connection_processor*> cb, int i)
////    {
//	std::cout<<"Connection_processor::set_cb(std::shared_ptr<Connection>)";
 //       cb_[i] = cb;
 //   }



}
