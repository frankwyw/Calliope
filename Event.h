#include "Connection.h"
#include <functional>

//  0,passive connection
//  1,active connection
//  2,read callback
//  3,write callback
//  4,passive close
//  5,active close
//  6,timeout

enum Event_Type
{
    PASSIV_CONN = 0,
    ACTIVE_CONN = 1,
    READ_CB = 2,
    WRITE_CB = 3,
    PASSIVE_CLOSE = 4,
    ACTIVE_CLOSE = 5,
    TIMEOUT = 6
}

class Event
{
public:
    //switch
    //PASSIV_CONN :reactor add_wait_fd,使用accepotr conn::passive_conn(conn_)
    //ACTIVE_CONN :reactor add_wait_fd，使用connector conn::active_conn(conn_)
    //READ_CB :conn_读取数据  conn::read(conn_);
    //WRITE_CB :conn_写入数据 conn::write(conn_);
    //PASSIVE_CLOSE :reactor 关闭conn conn::passive_close(conn_)
    //ACTIVE_CLOSE :reactor 关闭所有conn conn::passive_close(conn_)
    //TIMEOUT :reactor 关闭conn conn::timeout(conn_)
    void handle();
private:
    std::function<void()> func_;
    Connection* conn_;
    Reactor* reactor;
    Socket* socket_ ;
    Event_Type type;
    int nice;
}

std::less<Event>
struct less
{
    constexpr bool operator() (const Event& e1, const Event e2)
    {
        if(e1.nice < e2.nice)
        {
            return true;
        }
        return false;
    }
}
