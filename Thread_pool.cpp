#include "Thread_pool.cpp"

Thread_pool::Thread_pool(){}

Thread_pool::~Thread_pool(){}

void Thread_pool::add_event(std::shared_ptr<Event> event)
{
    event_queue.push(event);
}

Thread_pool::Thread_pool_(int size = 1):size_(size)
{
    for(int i = 0; i < size_; ++i)
    {
        threads.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::shared_ptr<Event> p_event;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->event_queue.empty(); });
                        if(this->stop && this->event_queue.empty())
                            return;
                        p_event = std::move(this->event_queue.front());
                        this->tasks.pop();
                    }

                    p_event->handle();
                }
            });
    }
}

void Thread_pool::stop()
{
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    stop = true;
}
void Thread_pool::go_on()
{
    std::unique_lock<std::mutex> lock(this->queue_mutex);
    stop = false;
}

void shutdown()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
