#include "Socket.h"


Socket::Socket(int fd):fd_(fd){}

Socket::~Socket(){ ::close(fd_); }

Socket::read(std::shared_ptr<Buffer> buffer){}

Socket::write(std::shared_ptr<Buffer> buffer){}
