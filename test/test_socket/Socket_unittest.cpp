#include "../../src/Socket.h"
#include "gtest/gtest.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace honoka;

TEST(Acceptor, firstCon)
{
	Socket s(100);
}

TEST(Accpetor, secondCon)
{

}

TEST(Acceptor, firstinit)
{
	
}

TEST(Acceptor, secondinit)
{
	Acceptor test(nullptr);
	int ip = inet_addr("127.0.0.1");
	test.init(ip, 10000, AF_INET, SOCK_STREAM);
	
}

