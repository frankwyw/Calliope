
class Connection_processor
{
public:
    virtual void set_new_connection(Connection*);
    Conection* clone();

    void passive_conn();
    void active_conn();
    void read();
    void write();
    void passive_close();
    void active_close();
    void timeout();
private:
    Connection* conn;
}

//抽象connection的处理为一个，将上下文（只有buffer？）传进去，这里的connection包含了conection的处理，将其抽象出来设为单例
//让客户继承编写自己的connection处理和connection_context
//甚至可以继承了以后使用python等脚本语言
