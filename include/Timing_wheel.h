#ifndef __TIMING_WHEEL__H__
#define __TIMING_WHEEL__H__

#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "Timing_Event_Type.hpp"

namespace honoka
{
	class Thread_pool;
	class Reactor;
	
	
	struct Timing_event;

	struct Timing
	{
		int fd_;
		int value_;
		int s_value;

		Timing();
	
		Timing(int fd, int value);
		~Timing();
	
	};

	class Timing_wheel
	{
	public:
	//create a new thread, work with run;
		Timing_wheel(Thread_pool* thread_pool, Reactor* reactor);
		~Timing_wheel();

	//get event, new, update, del
		void add_timing_event(const struct Timing_event& event);

	//sleep,get mutex,process event, send timeout to thread_pool
		void run();

	private:
		std::queue<struct Timing_event> queue_;
		std::mutex mutex_;
		

		void create_timing(int fd);
		void del_timing(int fd);
		int get_timeout();
		void advance();
		void update_timing(int fd);

		void skip_within_time();
		void install_new_timing(struct Timing& timing);
		void install_s_timing_to_ms();
		
		std::vector<std::vector<int>> ms_vec;
		std::vector<std::vector<int>> s_vec;

		std::vector<struct Timing> vec_;

		
		std::vector<std::vector<int>>::iterator cur_ms;
		std::vector<std::vector<int>>::iterator cur_s;

		int cur_ms_num;
		int cur_s_num;

		std::vector<int>::iterator cur;
		std::vector<int>::iterator cur_ed;

		Thread_pool* thread_pool_;
		Reactor* reactor_;

		bool is_stop;
		void stop();

		std::condition_variable cv;

		std::thread thread_;
	};
}


#endif
