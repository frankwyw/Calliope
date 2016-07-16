#include "Reactor.h"

#include <algorithm>

#include "Epoller.h"
#include "Configuration.h"
#include "Thread_pool.h"
#include "Socket.h"
#include "Event.h"
#include "Event_Type.hpp"
#include "Connection.h"
#include "Connection_processor.h"



namespace honoka
{
    Reactor::Reactor(Configuration*  config, int thread_pool_size = 1):config_(config)
        ,epoller_(std::make_shared<Epoller>(this)), fd_sockets_conns()
	, thread_pool_(std::make_shared<Thread_pool>(thread_pool_size))
	,conn_processor_(std::make_shared<Connection_processor>(this))
	,is_stop(false),mutex_(),is_close(false), cv()
    {

    }

    void Reactor::stop()
    {
        std::unique_lock<std::mutex> lock_(mutex_);
        is_stop = true;
    }

    void Reactor::go_on()
    {
        std::unique_lock<std::mutex> lock_(mutex_);
        is_stop = false;
    }

    void Reactor::shutdown()
    {
        std::unique_lock<std::mutex> lock_(mutex_);
        is_close = true;
    }

    void Reactor::close_listenning()
    {
        epoller_->close_listenning();
    }


    void Reactor::add_wait(std::shared_ptr<Socket> socket)
    {
        int fd = socket->get_fd();
        epoller_->add_wait(socket);
        auto tmp_conn = std::make_shared<Connection>(socket);
        fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
    }

    void Reactor::del_wait(std::shared_ptr<Socket> socket)
    {
        epoller_->del_wait(socket);
        auto ite = fd_sockets_conns.find(socket->get_fd());
        if(ite != fd_sockets_conns.end())
            fd_sockets_conns.erase(ite);
    }

    void Reactor::add_event(std::shared_ptr<Event> event)
    {
        thread_pool_->add_event(event);
    }


    void Reactor::add_listen(std::shared_ptr<Socket> socket)
    {
        int fd = socket->get_fd();
        epoller_->add_listen(socket);
        auto tmp_conn = std::make_shared<Connection>(socket);
        fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
    }

    void Reactor::loop()
    {
        for(;;)
        {
            {
                std::unique_lock<std::mutex> lock_(mutex_);
                cv.wait(lock_, [this](){return is_close || (!is_stop);});
                if(is_close)
                    break;
            }
            epoller_->run(0, thread_pool_.get(), fd_sockets_conns);
        }

    }

    std::shared_ptr<Event> Reactor::create_event(int fd, Event_Type type)
    {
        auto ite = fd_sockets_conns.find(fd);
        if(ite == fd_sockets_conns.end())
        {
    	    auto socket = std::make_shared<Socket>(fd);
            auto tmp_conn = std::make_shared<Connection>(socket);
    	    ite = fd_sockets_conns.emplace(fd, tmp_conn).first;
        }

        auto tmp = std::make_shared<Event>(this, ite->second, type);

        return tmp;
    }

    std::shared_ptr<Event> Reactor::create_new_conn_event(std::shared_ptr<Socket> socket, Event_Type type)
    {

        auto tmp_conn = std::make_shared<Connection>(socket);
        auto ite = fd_sockets_conns.emplace(socket->get_fd(), tmp_conn).first;
        auto tmp_ev = std::make_shared<Event>(this, ite->second, type);

        return tmp_ev;
    }

    void Reactor::set_cb(std::function<void (Connection_processor*, std::shared_ptr<Connection>)> cb, Event_Type type)
    {
        conn_processor_->set_cb(cb, static_cast<int>(type));
    }

    std::shared_ptr<Connection_processor> Reactor::get_conn_process()
    {
        return conn_processor_;
    }
}
