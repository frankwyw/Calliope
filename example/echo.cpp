#include <glog/logging.h>
#define DEBUG_MODE

#include "Tcp_server.h"
#include "Socket.h"
#include "Connection_processor.h"
#include <signal.h>
#include <iostream>
#include <functional>
#include "Connection.h"
#include "Buffer.hpp"
#include "Reactor.h"

honoka::Tcp_server* serv_ptr;

void stop(int sig)
{
	if(sig == 0)
		return;
	if(serv_ptr != nullptr)
		return;
	serv_ptr->shutdown();

}

using namespace honoka;

class My_processor : public Connection_processor
{
public:
	My_processor()
	{
		Connection_processor();

		using std::placeholders::_2;
		cb_[2] = std::bind(&My_processor::my_read_cb,this,_2);
		cb_[3] = std::bind(&My_processor::my_write_cb, this, _2);
		cb_[4] = std::bind(&My_processor::my_pass_close, this, _2);
	}

	void handle(honoka::Connection* conn, honoka::Event_Type type, honoka::Reactor* reactor_) override
	{
			//std::cout<<"please define processor handle(): "<<static_cast<int>(type)<<std::endl;
        	//用户定义std::bind(func_, _1, _2);
        	cb_[static_cast<int>(type)](this, conn);
			if(conn == nullptr)
        	{
				return;
			}
			auto tmp_socket = conn->socket;
			std::shared_ptr<Event> tmp_event;

        	switch (type) {
        	    case PASSIV_CONN:
        	    case ACTIVE_CONN:
        	        break;
        	    case READ_CB:
					tmp_event = reactor_->create_event(tmp_socket->get_fd(), WRITE_CB);
					reactor_->add_event(tmp_event);
					break;
        	    case WRITE_CB:
        	        break;
        	    case PASSIVE_CLOSE:
        	    case ACTIVE_CLOSE:
        	        break;
        	    case TIMEOUT:
			reactor_->del_wait(tmp_socket->get_fd());
        	        break;
        	    default:
         	       break;
        	}
	}
private:
	void my_read_cb(honoka::Connection* conn)
	{
		auto buffer = conn->buffer;
		auto socket = conn->socket;
		buffer->read(socket);
	//	char buf[100];
	//	int size = buffer->write(buf, 10);
	//	buf[size] = '\0';
	//	std::cout<<buf<<std::endl;
		std::cout<<"my_read_cb"<<std::endl;
	}

	void my_write_cb(honoka::Connection* conn)
	{
		auto buffer = conn->buffer;
		auto socket = conn->socket;
		int size = buffer->write(socket);
		std::cout<<"send"<<size<<" bytes"<<std::endl;
	}

	void my_pass_close(honoka::Connection* conn)
	{
		std::cout<<"pass_close"<<std::endl;
	}
};

int main()
{

	google::InitGoogleLogging("honoka.log");
	honoka::Tcp_server server(1);
	serv_ptr = &server;
//	signal(SIGINT, stop);
	std::string a = "init.json";
	try
	{
		server.init(a);
	}
	catch(std::exception& e)
	{
		std::cout<<e.what()<<std::endl;
		exit(1);
	}
	
	std::shared_ptr<Connection_processor> my_processor = std::make_shared<My_processor>();
	server.set_conn_processor(my_processor);
	server.run();

	google::ShutdownGoogleLogging();
}
