#include "Event.h"

#include "Connection.h"
#include "Reactor.h"
#include "Connection_processor.h"

#include <glog/logging.h>


namespace honoka
{
    int Event::type_to_nice[7] = {6,6,5,5,4,4,4};
    Event::Event(Reactor* reactor, Connection* conn, Event_Type type):conn_(conn), reactor_(reactor), type_(type)
    {
	if(reactor_ == nullptr)
	{
		LOG(ERROR)<<"Event::Event() reactor == nullptr";
	}
	if(conn == nullptr)
	{
		LOG(ERROR)<<"Event::Event() conn == nullptr";
	}
        conn_processor_ = reactor->get_conn_process().get();
        nice = type_to_nice[static_cast<int>(type)];
    }

    void Event::handle()
    {
        conn_processor_->handle(conn_, type_, reactor_);
    }

}
