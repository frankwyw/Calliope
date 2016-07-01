#include "Socket.h"
#include "Event.h"
#include <vector>

//  0,passive connection
//  1,active connection
//  2,read callback
//  3,write callback
//  4,passive close
//  5,active close
//  6,timeout
//  vector中的6个事件回调，默认指向一个初始化表（共享模式？！）

class Connection
{
public:
    void passive_conn();
    void active_conn();
    void read();
    void write();
    void passive_close();
    void active_close();
    void timeout();
private:
    Socket* socket;
    Buffer buffer;
    Reacotr* reacotr;
//    Socket socket_;
//    std::vector<std::shared_ptr<Event>> cb;

}
