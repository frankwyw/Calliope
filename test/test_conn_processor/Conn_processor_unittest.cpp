#include "Connection_processor.h"
#include "Event_Type.hpp"
#include <memory>

using namespace honoka;

int main()
{
	Connection_processor test;
	Event_Type type = ACTIVE_CONN;
	test.handle(type);
}
