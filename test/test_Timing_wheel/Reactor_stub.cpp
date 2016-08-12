#include "Reactor.h"

#include <algorithm>

#include <glog/logging.h>

//#include "Epoller.h"
//#include "Configuration.h"
//#include "Thread_pool.h"
//#include "Socket.h"
//#include "Event.h"
#include "Event_Type.hpp"
//#include "Connection.h"
//#include "Connection_processor.h"

namespace honoka
{
    
class Epoller{};
class Configuration{};
class Thread_pool{};
class Socket{};
class Event{};
class Connectin{};
class Connectin_processor{};

	Reactor::Reactor(Configuration*  config_, int thread_pool_size){}

    Reactor::~Reactor(){};

    void Reactor::init()
    {
        //epoller_->init();
    }

    void Reactor::stop()
    {
    }

    void Reactor::go_on()
    {
    }

    void Reactor::shutdown()
    {
    }

    void Reactor::close_listenning()
    {
    }

    void Reactor::set_conn_processor(std::shared_ptr<Connection_processor> conn_processor)
    {
        
    }


    void Reactor::add_event(std::shared_ptr<Event> event)
    {

    }


    void Reactor::add_listen(std::shared_ptr<Socket> socket)
    {
        
    }

    void Reactor::del_listen(std::shared_ptr<Socket> socket)
    {
        
    }


    void Reactor::loop()
    {
        
    }

    std::shared_ptr<Event> Reactor::create_event(int fd, Event_Type type)
    {
        auto tmp = std::make_shared<Event>();

        return tmp;
    }

    std::shared_ptr<Event> Reactor::create_new_conn_event(std::shared_ptr<Socket> socket, Event_Type type)
    {
        
        auto tmp_ev = std::make_shared<Event>();

        return tmp_ev;
    }

    void Reactor::set_cb(std::function<void (Connection_processor*, Connection*)> cb, Event_Type type)
    {
        
    }

    std::shared_ptr<Connection_processor> Reactor::get_conn_process()
    {
        return conn_processor_;
    }
}
