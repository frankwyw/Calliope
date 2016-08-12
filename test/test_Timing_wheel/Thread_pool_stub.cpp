#include "Thread_pool.h"
#include "Event.h"

#include <glog/logging.h>

namespace std
{
	bool less<std::shared_ptr<honoka::Event>>::operator()
		(const std::shared_ptr<honoka::Event>&e1, const std::shared_ptr<honoka::Event>&e2) const
	{
		return *e1< *e2;
	}

}


namespace honoka
{
	Thread_pool::Thread_pool(int size){}

	Thread_pool::~Thread_pool()
	{
	  
	}

	void Thread_pool::add_event(std::shared_ptr<Event> event)
	{
	   
	}


	void Thread_pool::stop()
	{
	    
	}

	void Thread_pool::go_on()
	{
	   
	}

	void Thread_pool::shutdown()
	{
	   
	}


}
