#include "Event.h"

#include "Connection.h"
#include "Reactor.h"
#include "Connection_processor.h"


namespace honoka
{
    int Event::type_to_nice[7] = {5,5,1,1,10,10,10};
    Event::Event(Reactor* reactor, std::shared_ptr<Connection> conn, Event_Type type):conn_(conn), reactor_(reactor), type_(type)
    {
        conn_processor_ = reactor->get_conn_process();
        nice = type_to_nice[static_cast<int>(type)];
    }

    void Event::handle()
    {
        Connection_processor->handle(conn_, type_);
    }

}
