#include "Thread_pool.h"
#include "Event.h"


namespace std
{
	bool less<std::shared_ptr<honoka::Event>>::operator() (const std::shared_ptr<honoka::Event>&e1, const std::shared_ptr<honoka::Event>&e2) const
	{
		return *e1< *e2;
	}

}


namespace honoka
{

Thread_pool::~Thread_pool(){}

void Thread_pool::add_event(std::shared_ptr<Event> event)
{
    event_queue.push(event);
}

Thread_pool::Thread_pool(int size = 1):size_(size)
{
    for(int i = 0; i < size_; ++i)
    {
        threads.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::shared_ptr<Event> p_event_ptr;

                    {
                        std::unique_lock<std::mutex> lock_(this->queue_mutex);
                        this->cond_var.wait(lock_,
                            [this]{ return this->is_stop || !this->event_queue.empty(); });
                        if(this->is_stop && this->event_queue.empty())
                            return;
                        p_event_ptr = this->event_queue.top();
                        this->event_queue.pop();
                    }

                    p_event_ptr->handle();
                }
            });
    }
}

void Thread_pool::stop()
{
    std::unique_lock<std::mutex> lock_(queue_mutex);
    is_stop = true;
}

void Thread_pool::go_on()
{
    std::unique_lock<std::mutex> lock_(queue_mutex);
    is_stop = false;
}

void Thread_pool::shutdown()
{
    {
        std::unique_lock<std::mutex> lock_(queue_mutex);
        is_stop = true;
    }
    cond_var.notify_all();
    for(std::thread &thread_: threads)
        thread_.join();
}


}
