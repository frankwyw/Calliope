#include "Connection.h"

namespace honoka
{

    Connection(Reactor* reactor, std::shared_ptr<Socket> socket):reactor_(reactor), socket_(socket)
    {

    }


}
