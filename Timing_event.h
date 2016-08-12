#ifdef __TIMING__EVENT__H__
#define __TIMING__EVENT__H__


//10 * 1000 ms
#define DEFAULT_TIME 10000


namespace honoka{

struct Timing_event
{
	Timing_type type_;
	//ms
	int fd_;
	Timing_event;
	~Timing_event;
};

struct Timing
{
	int fd_;
	int value_;
	int s_value;

	Timing();
	
	Timing(int fd, int value);
	~Timing();
	
};

}


#endif
