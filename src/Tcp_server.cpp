#include "Tcp_server.h"

#include "Configuration.h"
#include "Reactor.h"
#include "Acceptor.h"
#include "Connection_processor.h"

#include <glog/logging.h>


namespace honoka
{
    Tcp_server::Tcp_server(int thread_pool_size):config_(std::make_shared<Configuration>())
	,reactor_(std::make_shared<Reactor>(config_.get(), 1))
	, acceptor_(std::make_shared<Acceptor>(reactor_.get(), config_.get()))
	{}

    //读取json，更新config，reactor,acceptor更新相关，包括各种事件的处理
    //acceptor设置监听，添加进epoller
    void Tcp_server::init(std::string configfilename)
    {
	    DLOG(INFO)<<"Tcp_server::init(std::string )";
        config_->init(configfilename);
        reactor_->init();
        acceptor_->init();
    }

    void Tcp_server::init()
    {
        DLOG(INFO)<<"Tcp_server::init()";
        config_->init();
        reactor_->init();
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

    void Tcp_server::set_conn_processor(std::shared_ptr<Connection_processor> conn_processor)
    {
        reactor_->set_conn_processor(conn_processor);
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
