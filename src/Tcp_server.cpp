

namespace honoka
{
    Tcp_server::Tcp_server():config_(),reactor_(config_), acceptor_(reactor_, config_){}

    Tcp_server::Tcp_server(int thread_pool_size):config_(),reactor_(config_, thread_pool_size), acceptor_(reactor_, config_){}

    //读取json，更新config，reactor,acceptor更新相关，包括各种事件的处理
    //acceptor设置监听，添加进epoller
    void Tcp_server::init(std::string configfilename)
    {
        config_->init(configfilename);
        acceptor_->init();
    }

    //
    void Tcp_server::run()
    {
        reactor_->loop();
    }

    //信号处理
    void Tcp_server::reset_config()
    {
        reactor_->stop();
        reactor_->close_listenning();
        acceptor_->init();
        reactor_->go_on();
    }


    void Tcp_server::stop()
    {
        reactor_->stop();
    }
    void Tcp_server::go_on()
    {
        reactor_->go_on();
    }
    void Tcp_server::shutdown()
    {
        reactor_->shutdown();
        //
    }
}
