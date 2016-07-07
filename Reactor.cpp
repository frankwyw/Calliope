#include "Reactor.h"

#include <algorithm>

#include "Epoller.h"
#include "Configuration.h"
#include "Thread_pool.h"
#include "Socket.h"
#include "Connection_processor.h"
#include "Event.h"


namespace honoka
{
void Reactor::add_wait(std::shared_ptr<Socket> socket)
{
    int fd = socket->get_fd();
    epoller_->add_wait(fd);
    auto tmp_conn = std::make_shared<Connection>(this, socket);
    fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
}

void Reactor::del_wait(int fd)
{
    epoller_->del_wait(fd);
    auto ite = fd_sockets_conns.find(fd);
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
    epoller_->add_listen(fd);
    auto tmp_conn = std::make_shared<Connection>(this, socket);
    fd_sockets_conns.insert(std::make_pair(fd, tmp_conn));
}

void Reactor::loop()
{

}

std::shared_ptr<Event> Reactor::create_event(int fd, int type)
{
    auto ite = fd_sockets_conns.find(fd);
    if(ite == fd_sockets_conns.end())
    {
        auto tmp_conn = std::make_shared<Connection>(this, std::make_shared<Socket>(fd));
//        ite = fd_sockets_conns.emplace(fd, tmp_conn);
    }

    auto tmp = std::make_shared<Event>(this, ite->second(), type);

    return tmp;
}
}
