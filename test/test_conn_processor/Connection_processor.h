#ifndef __CONNECTION__PROCESSOR__H__
#define __CONNECTION__PROCESSOR__H__

#include <memory>
#include <functional>
#include "Event_Type.hpp"

namespace honoka
{
 //   class Reactor;
//    class Connection;

    class Connection_processor
    {
    public:


        Connection_processor();
        ~Connection_processor();
        void handle( Event_Type type);

 //       void set_cb(std::function<void (Connection_processor*> cb, int i);

    private:
        void passive_conn();
        void active_conn();
        void read();
        void write();
        void passive_close();
        void active_close();
        void timeout();

        std::function<void (Connection_processor*)> cb_[7];

//        Reactor* reactor_;
    };
}

//抽象connection的处理为一个，将上下文（只有buffer？）传进去，这里的connection包含了conection的处理，将其抽象出来设为单例
//让客户继承编写自己的connection处理和connection_context
//甚至可以继承了以后使用python等脚本语言

#endif
