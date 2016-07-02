#include "Epoller.h"
#include "Configuration.h"
#include "Thread_pool.h"
#include "Socket.h"
#include "Connection_processor.h"

class Reactor
{
public:

    //为epoller添加监听事件
    //新链接更新socket_conn
    void add_wait(int );

    void del_wait(int );

    //添加监听套接字
    inline void add_listen(int )
    {
        epoller->add_listen(socket);
    }

    inline void add_event(Event* event)
    {
        thread_pool_->add_event(event);
    }

    //运行epoller
    void loop();

    void set_conn_process();
    Connection_processor& get_conn_process();

private:
    //添加event到Thread_pool处理


    std::shared_ptr<Connection_processor> conn_processor;

    Configuration*  config_;
    std::shared_ptr<Epoller> epoller_;
    std::vector<std::shared_ptr<Socket>> sockets;
    std::map<int, std::shared_ptr<Connection>>   socket_conns;
    std::shared_ptr<Thread_pool> thread_pool_;
    std::Thread_priority_queue<Socket*> time_heap;
}
