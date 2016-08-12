#include "Reactor.h"

#include <algorithm>

#include <glog/logging.h>


#include "Epoller.h"
#include "Configuration.h"
#include "Thread_pool.h"
#include "Socket.h"
#include "Event.h"
#include "Event_Type.hpp"
#include "Connection.h"
#include "Connection_processor.h"

#include "Timing_wheel.h"
#include "Timing_Event_Type.hpp"
#include "Timing_event.h"



namespace honoka
{
    Reactor::Reactor(Configuration*  config, int thread_pool_size = 1):config_(config), fd_sockets_conns()
    ,epoller_(std::make_shared<Epoller>(this, &fd_sockets_conns))
	,thread_pool_(std::make_shared<Thread_pool>(thread_pool_size))
	,conn_processor_(std::make_shared<Connection_processor>())
	,is_stop(false),mutex_(),is_close(false), cv(),timing_wheel_(std::make_shared<struct Timing_wheel>(thread_pool_.get(), this))
    {

    }

    Reactor::~Reactor(){};

    void Reactor::init()
    {
        epoller_->init();
    }

    void Reactor::stop()
    {
        DLOG(INFO)<<" Reactor::stop()";
        std::unique_lock<std::mutex> lock_(mutex_);
        is_stop = true;
    }

    void Reactor::go_on()
    {
        DLOG(INFO)<<" Reactor::go_on()";
        std::unique_lock<std::mutex> lock_(mutex_);
        is_stop = false;
    }

    void Reactor::shutdown()
    {
        DLOG(INFO)<<" Reactor::shutdown()";
        std::unique_lock<std::mutex> lock_(mutex_);
        is_close = true;
    }

    void Reactor::close_listenning()
    {
        DLOG(INFO)<<" Reactor::close_listenning()";
        epoller_->close_listenning();
    }

    void Reactor::set_conn_processor(std::shared_ptr<Connection_processor> conn_processor)
    {
        conn_processor_ = conn_processor;
    }


  //  void Reactor::add_wait(std::shared_ptr<Socket> socket)
  //  {
 //	DLOG(INFO)<<" Reactor::add_wait(std::shared_ptr<Socket> socket)";
 //       int fd = socket->get_fd();
 //       epoller_->add_wait(socket);
 //       auto tmp_conn = std::make_shared<Connection>(socket);
 //       fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
 //   }

 //   void Reactor::del_wait(std::shared_ptr<Socket> socket)
 //   {
//	DLOG(INFO)<<" Reactor::del_wait(std::shared_ptr<Socket> socket)";
 //       epoller_->del_wait(socket);
 //       auto ite = fd_sockets_conns.find(socket->get_fd());
 //       if(ite != fd_sockets_conns.end())
 //           fd_sockets_conns.erase(ite);
  //  }

    void Reactor::add_event(std::shared_ptr<Event> event)
    {
        DLOG(INFO)<<" Reactor::add_event(std::shared_ptr<Event> event)";
        thread_pool_->add_event(event);
    }


    void Reactor::add_listen(std::shared_ptr<Socket> socket)
    {
        DLOG(INFO)<<" Reactor::add_listen(std::shared_ptr<Socket> socket)";
        int fd = socket->get_fd();
        epoller_->add_listen(socket);
        auto tmp_conn = std::make_shared<Connection>(socket);
        fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
    }

    void Reactor::del_listen(std::shared_ptr<Socket> socket)
    {
        DLOG(INFO)<<" Reactor::del_listen(std::shared_ptr<Socket> socket)";
        int fd = socket->get_fd();
        epoller_->del_listen(socket);
        auto ite = fd_sockets_conns.find(socket->get_fd());
        if(ite != fd_sockets_conns.end())
            fd_sockets_conns.erase(ite);
    }


    void Reactor::loop()
    {
        for(;;)
        {
//	DLOG(INFO)<<" Reactor::loop() new loop";
            {
                std::unique_lock<std::mutex> lock_(mutex_);
                cv.wait(lock_, [this](){return is_close || (!is_stop);});
                if(is_close)
                    break;
            }
            epoller_->run(0, thread_pool_.get());
        }

    }

    std::shared_ptr<Event> Reactor::create_event(int fd, Event_Type type)
    {
        DLOG(INFO)<<" Reactor::create_event() ";

        auto ite = fd_sockets_conns.find(fd);
        Connection* conn = nullptr;
        if(ite != fd_sockets_conns.end())
        {
    	    conn = ite->second.get();
        }

        auto tmp = std::make_shared<Event>(this, conn, type);

        return tmp;
    }

    std::shared_ptr<Event> Reactor::create_new_conn_event(std::shared_ptr<Socket> socket, Event_Type type)
    {
        DLOG(INFO)<<" Reactor::create_new_conn_event ";
        auto tmp_conn = std::make_shared<Connection>(socket);
        auto ite2 = fd_sockets_conns.insert(std::make_pair(socket->get_fd(), tmp_conn));
        auto ite = ite2.first;
        auto tmp_ev = std::make_shared<Event>(this, tmp_conn.get(), type);

        return tmp_ev;
    }

    void Reactor::set_cb(std::function<void (Connection_processor*, Connection*)> cb, Event_Type type)
    {
        DLOG(INFO)<<" Reactor::set_cb() ";
        conn_processor_->set_cb(cb, static_cast<int>(type));
    }

    std::shared_ptr<Connection_processor> Reactor::get_conn_process()
    {
        DLOG(INFO)<<" Reactor::get_conn_process() ";

        return conn_processor_;
    }

	void Reactor::install_Timing_wheel(int fd)
	{
		Timing_event tev(NEW_TIMING, fd);
		timing_wheel_->add_timing_event(tev);
	}
	void Reactor::update_Timing_wheel(int fd)
	{
		Timing_event tev(UPDATE_TIMING, fd);
		timing_wheel_->add_timing_event(tev);
	}
	void Reactor::del_Timing_wheel(int fd)
	{
		Timing_event tev(DEL_TIMING, fd);
		timing_wheel_->add_timing_event(tev);
	}
	
	void Reactor::del_wait(int fd)
	{
		epoller_->del_wait_fd(fd);
	}
}



