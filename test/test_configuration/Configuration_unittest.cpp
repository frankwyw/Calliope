#include "../../src/Configuration.h"
#include "gtest/gtest.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace honoka;

TEST(Configuration, firstCon)
{
	Configuration test;
}

TEST(Configuration, firstinit)
{
	Configuration test;
	test.init();
}

TEST(Configuration, secondCon)
{
	Configuration test;
	test.init("init.json");
}

TEST(Configuration, get_socket_num)
{
	Configuration test;
	test.init("init.json");
	EXPECT_EQ(1, test.get_socket_num());
}


TEST(Configuration, get_ip)
{
	Configuration test;
	test.init("init.json");
	EXPECT_STREQ("127.0.0.1", test.get_ip(0).c_str());
}


TEST(Configuration, get_port)
{
	Configuration test;
	test.init("init.json");
	EXPECT_EQ(10008, test.get_port(0));
}





