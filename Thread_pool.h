#include <priority_queue>
#include <condition_variable>
#include <mutex>
#include "Event.h"

class Thread_pool
{
public:
    void add_events(std::shared_ptr<Event> event);
    void stop();
    void go_on();
    void shutdown();

    Thread_pool_(int size = 1);
private:
    sted::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>,
        [](std::shared_ptr<Event>&e1, std::shared_ptr<Event>&e2){return std::less<Event>(*e1, *e2);}> event_queue;
    std::condition_variable cond_var;
    std::mutex queue_mutex;
    std::vector<std::thread<>> threads;
    int size_;

    bool stop;
}
