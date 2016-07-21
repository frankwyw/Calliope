#include "../../src/Thread_pool.h"
#include "Event.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>

using namespace honoka;



int main()
{
	Thread_pool test(1);
	auto ev = std::make_shared<Event>(nullptr, nullptr, TIMEOUT);
	test.add_event(ev);
	auto ev2 = std::make_shared<Event>(nullptr, nullptr, READ_CB);
	test.add_event(ev2);
}




