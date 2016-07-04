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

    inline void add_event(std::shared_ptr<Event> event)
    {
        thread_pool_->add_event(event);
    }

    std::shared_ptr<Event> create_event(int fd, int type);


    //运行epoller
    void loop();

    void set_conn_process();
    Connection_processor& get_conn_process();

private:
    //添加event到Thread_pool处理

    bool upate_fd_sockets_conns(int fd);

    std::shared_ptr<Connection_processor> conn_processor;

    Configuration*  config_;
    std::shared_ptr<Epoller> epoller_;

    std::map<int, std::pair<std::shared_ptr<Socket>, std::shared_ptr<Connection>>>   fd_sockets_conns;
    std::shared_ptr<Thread_pool> thread_pool_;
    std::Thread_priority_queue<Socket*> time_heap;
}
