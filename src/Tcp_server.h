#ifndef __TCP__SERVER__H__
#define __TCP__SERVER__H__

namespace honoka
{
    class Reactor;
    class Acceptor;
    class Configuration;

    class Tcp_server
    {
    public:
        //config_()默认初始化
        Tcp_server():config_(),reactor_(config_), acceptor_(reactor_, config_){}

        Tcp_server(int thread_pool_size):config_(),reactor_(config_, thread_pool_size), acceptor_(reactor_, config_){}

        //读取json，更新config，reactor,acceptor更新相关，包括各种事件的处理
        //acceptor设置监听，添加进epoller
        void init();

        //
        void run();


    private:
        //信号处理
        void reset_config()
        {
            reactor.stop();
            reactor.close_listenning();
            acceptor.init();
            reactor.go_on();
        }
        void stop();
        void go_on();
        void shutdown()
        {
            reactor.shutdown();
            //
        }

        std::shared_ptr<Configuration> config_;
        std::shared_ptr<Reactor> reactor_;
        std::shared_ptr<Accpetor> acceptor_;
    };
}

#endif
