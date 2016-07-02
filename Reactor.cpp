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
