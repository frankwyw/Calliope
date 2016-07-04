#include "Thread_priority_queue.h"

Thread_priority_queue::Thread_priority_queue( const Compare& compare, const Container& cont ):queue(comp, cont), mutex_(){};

explicit Thread_priority_queue::Thread_priority_queue( const Compare& compare = Compare(),Container&& cont = Container() )::queue(comp, cont),mutex_(){};

//Thread_priority_queue::Thread_priority_queue( const Thread_priority_queue& other ):comp(other.comp), queue(other.queue), mutex_(){};

Thread_priority_queue::Thread_priority_queue( Thread_priority_queue&& other ):comp(other.comp), queue(other.queue), mutex_(std::move(other.mutex_))){};

//没有添加allcotor部分

template< class InputIt >
Thread_priority_queue::Thread_priority_queue( InputIt first, InputIt last, const Compare& compare, const Container& cont ):cont(first, last, compare, cont),mutex_(){}

template< class InputIt >
Thread_priority_queue::Thread_priority_queue( InputIt first, InputIt last, const Compare& compare = Compare(), Container&& cont = Container() ):cont(first, last, compare, cont),mutex_(){};

Thread_priority_queue::~Thread_priority_queue(){}

//Thread_priority_queue Thread_priority_queue::operator=( const Thread_priority_queue& other );
//Thread_priority_queue Thread_priority_queue::operator=( Thread_priority_queue&& other );


const_reference top() const
{
    return queue.top();
}
bool Thread_priority_queue::empty() const
{
    std::lock_guard<std::mutex> lk(mutex_);
    return queue.empty();
}
size_type Thread_priority_queue::size() const
{
    std::lock_guard<std::mutex> lk(mutex_);
    return queue.size();
}

void Thread_priority_queue::push( const T& value )
{
    std::lock_guard<std::mutex> lk(mutex_);
    queue.push(value);
}
void Thread_priority_queue::push( T&& value )
{
    std::lock_guard<std::mutex> lk(mutex_);
    queue.push(value);
}

template< typename... Args >
void Thread_priority_queue::emplace( Args&&... args )
{
    std::lock_guard<std::mutex> lk(mutex_);
    queue.emplace(std::forward<Args...> (args...));
}

void Thread_priority_queue::pop()
{
    std::lock_guard<std::mutex> lk(mutex_);
    queue.pop();
}

void Thread_priority_queue::swap(Thread_priority_queue& other)
{
    std::lock_guard<std::mutex> lk(mutex_);
    queue.swap(other);
}
