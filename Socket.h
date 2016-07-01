

class Socket
{
public:
    Socket(){}
    ~Socket(){}

    void read();
    void write();

    int get_fd();
private:
    int fd;
}


class Time_Socket : public Socket
{
public:
    int get_time();
    void set_time();

private:
    int time;
}
