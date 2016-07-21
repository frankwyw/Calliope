#ifndef __REACTOR__H__
#define __REACTOR__H__

//#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "Event_Type.hpp"

namespace honoka
{
    class Epoller;
    class Configuration;
    class Thread_pool;
    class Socket;
    class Event;
    class Connection;
    class Connection_processor;

    class Reactor
    {
    public:

        Reactor(Configuration*  config_, int thread_pool_size);
        ~Reactor();

        void init();

        //为epoller添加监听事件
        //新链接更新socket_conn
//        void add_wait(std::shared_ptr<Socket> socket);

 //       void del_wait(std::shared_ptr<Socket> socket);


        //添加监听套接字
        void add_listen(std::shared_ptr<Socket> socket);
        void del_listen(std::shared_ptr<Socket> socket);

        void add_event(std::shared_ptr<Event> event);

        void set_cb(std::function<void (Connection_processor*, Connection*)> cb, Event_Type type);

        void set_conn_processor(std::shared_ptr<Connection_processor> conn_processor);


//        std::shared_ptr<Event> create_event(std::shared_ptr<Socket> socket, Event_Type type);


        //运行epoller
        void loop();

        void stop();
        void go_on();
        void shutdown();
        void close_listenning();

        std::shared_ptr<Connection_processor> get_conn_process();

        //添加event到Thread_pool处理
        std::shared_ptr<Event> create_event(int fd, Event_Type type);
        std::shared_ptr<Event> create_new_conn_event(std::shared_ptr<Socket> socket, Event_Type type);
    private:

        bool is_stop;
        bool is_close;
        std::mutex mutex_;
        std::condition_variable cv;

        Configuration*  config_;
	      std::map<int, std::shared_ptr<Connection>>   fd_sockets_conns;
        std::shared_ptr<Epoller> epoller_;


        std::shared_ptr<Thread_pool> thread_pool_;
        std::shared_ptr<Connection_processor> conn_processor_;
    };
}

#endif
