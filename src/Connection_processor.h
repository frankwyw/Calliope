#ifndef __CONNECTION__PROCESSOR__H__
#define __CONNECTION__PROCESSOR__H__

#include <memory>
#include <functional>
#include "Event_Type.hpp"

namespace honoka
{
    class Reactor;
    class Connection;

    class Connection_processor
    {
    public:


        Connection_processor();
        ~Connection_processor();
	    virtual void handle(Connection* conn, Event_Type type, Reactor* reactor);

        void set_cb(std::function<void (Connection_processor*, Connection*)> cb, int i);

    protected:
        void passive_conn(Connection* conn);
        void active_conn(Connection* conn);
        void read(Connection* conn);
        void write(Connection* conn);
        void passive_close(Connection* conn);
        void active_close(Connection* conn);
        void timeout(Connection* conn);

        std::function<void (Connection_processor*, Connection*)> cb_[7];

    };
}

//抽象connection的处理为一个，将上下文（只有buffer？）传进去，这里的connection包含了conection的处理，将其抽象出来设为单例
//让客户继承编写自己的connection处理和connection_context
//甚至可以继承了以后使用python等脚本语言

#endif
