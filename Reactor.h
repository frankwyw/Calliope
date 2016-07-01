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
    void add_wait_fd();
    void del_wait_fd();

    //添加监听套接字
    int add_listen_fd(int );

    //运行epoller
    void loop();

    void set_base_conn();
    Connection& get_base_conn();

private:
    //添加event到Thread_pool处理
    void add_event();

    std::shared_ptr<Connection_processor> conn_processor;

    Configuration*  config_;
    std::shared_ptr<Epoller> epoller_;

    std::map<std::shared_ptr<Socket>, std::shared_ptr<Connection>>   socket_conns;
    std::shared_ptr<Thread_pool> thread_pool_;
    std::Thread_priority_queue<Socket*> time_heap;
}
