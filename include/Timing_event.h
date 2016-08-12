#ifndef __TIMING__EVENT__H__
#define __TIMING__EVENT__H__

#include "Timing_Event_Type.hpp"

namespace honoka{
	struct Timing_event
	{
		Timing_type type_;
		//ms
		int fd_;
		Timing_event(Timing_type type, int fd);
		~Timing_event();
	};
}

#endif
