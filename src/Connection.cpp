#include "Connection.h"
#include "Buffer.hpp"
#include <memory>

honoka::Connection::Connection(std::shared_ptr<Socket> socket_):socket(socket_), buffer(std::make_shared<Ring_buffer<char>>(1000))
{

}
