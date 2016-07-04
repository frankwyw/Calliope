#include "Reactor.h"
#include <algorithm>

void Reactor::add_wait(Socket* socket)
{
    epoller->add_wait(socket);
}

void Reactor::del_wait(Socket* socket)
{
    epoller->del_wait(socket);
}

void Reactor::loop()
{

}

std::shared_ptr<Event> Reactor::create_event(int fd, int type)
{
    auto ite = fd_sockets_conns.find(fd);
    if(ite == fd_sockets_conns.end())
    {
        upate_fd_sockets_conns(fd);
    }

    auto tmp = std::make_shared<Event>(type, pair.first.get(), pair.second.get());

    return tmp;
}

void Reactor::upate_fd_sockets_conns(int fd)
{
    auto first = std::make_shared<Socket>(fd);
    auto second = std::make_shared<Connection>(fd);
    auto pair_ = std::make_pair(first, second);
    ite = fd_sockets_conns.insert(pair_);
}
