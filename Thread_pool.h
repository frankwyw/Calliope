#ifndef __THREAD__POOL__H__
#define __THREAD__POOL__H__

#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <memory>

namespace honoka
{
     class Event;
}

namespace std
{
template<>
	struct less<std::shared_ptr<honoka::Event>>
	{
		bool operator()(const std::shared_ptr<honoka::Event>&e1, const std::shared_ptr<honoka::Event>&e2) const;
	};
}

namespace honoka
{
    

    class Thread_pool
    {
    public:
        void add_event(std::shared_ptr<Event> event);
        void stop();
        void go_on();
        void shutdown();

        Thread_pool(int size);
	~Thread_pool();
    private:
        std::priority_queue<std::shared_ptr<Event>> event_queue;
        std::condition_variable cond_var;
        std::mutex queue_mutex;
        std::vector<std::thread> threads;
        int size_;

        bool is_stop;
    };
}



#endif
