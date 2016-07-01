#include "Thread_priority_queue.h"
#include "Event.h"

class Thread_pool
{
public:
    void add_events();
    void run();
    void stop();
    void go_on();
    void shutdown();
private:
    Thread_priority_queue<Event> event_queue;
}
