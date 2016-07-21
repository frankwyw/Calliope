#include "Event.h"

#include <iostream>

namespace honoka
{
    class Connection{};
    class Reactor{};
    class Connection_processor{};
}



namespace honoka
{
	
    int Event::type_to_nice[7] = {5,5,1,1,10,10,10};
    Event::Event(Reactor* reactor, std::shared_ptr<Connection> conn, Event_Type type):conn_(conn), reactor_(reactor), type_(type)
    {
        //conn_processor_ = reactor->get_conn_process().get();
	std::cout<<"Event::Event"<<std::endl;
        nice = type_to_nice[static_cast<int>(type)];
    }

    void Event::handle()
    {
	std::cout<<"Event::handle"<<type_<<std::endl;
        //conn_processor_->handle(conn_, type_);
    }

}
