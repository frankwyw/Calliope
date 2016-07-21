#ifndef __TCP__SERVER__H__
#define __TCP__SERVER__H__

#include <memory>

namespace honoka
{
    class Reactor;
    class Acceptor;
    class Configuration;
    class Connection_processor;

    class Tcp_server
    {
    public:

        Tcp_server(int thread_pool_size = 1);

        //读取json，更新config，reactor,acceptor更新相关，包括各种事件的处理
        //acceptor设置监听，添加进epoller
        void init(std::string configfilename);

        void init();

        //
        void run();

        void set_conn_processor(std::shared_ptr<Connection_processor> conn_processor);


            //信号处理
        void reset_config();

        void stop();
        void go_on();
        void shutdown();

    private:

        std::shared_ptr<Configuration> config_;
        std::shared_ptr<Reactor> reactor_;
        std::shared_ptr<Acceptor> acceptor_;
    };
}

#endif
