#include "Connection_processor.h"

#include <memory>

#include "Event.h"
#include "Reactor.h"
#include "Connection.h"
#include "Socket.h"
#include "Buffer.h"


namespace honoka
{


Connection_processor::Connection_processor(Reactor* reactor):reactor_(reactor){}

void Connection_processor::handle(std::shared_ptr<Connection> conn)
{

}

void Connection_processor::passive_conn(std::shared_ptr<Connection> conn)
{
//accpet conn
//add_new
//LOGSTREAM

    auto tmp_socket = conn->get_socket();
    reactor_->add_wait(tmp_socket);

}
void Connection_processor::active_conn(std::shared_ptr<Connection> conn)
{

}
void Connection_processor::read(std::shared_ptr<Connection> conn)
{
    auto tmp_socket = conn->get_socket();
    tmp_socket->read();
}
void Connection_processor::write(std::shared_ptr<Connection> conn)
{
    auto tmp_socket = conn->get_socket();
    tmp_socket->write();
}
void Connection_processor::passive_close(std::shared_ptr<Connection> conn)
{
    auto tmp_socket = conn->get_socket();
    reactor_->del_listen(tmp_socket->get_fd());
}
void Connection_processor::active_close(std::shared_ptr<Connection> conn)
{
    auto tmp_socket = conn->get_socket();
    reactor_->del_listen(tmp_socket->get_fd());
}
void Connection_processor::timeout(std::shared_ptr<Connection> conn)
{
    auto tmp_socket = conn->get_socket();
    reactor_->del_wait(tmp_socket);
}

}


