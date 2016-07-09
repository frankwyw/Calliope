#define MAXEPOLL 1024

#include "Epoller.h"

#include <memory>
#include <sys/epoll.h>

#include "Event.h"
#include "Reactor.h"
#include "tool_function.hpp"
#include "Event_Type.hpp"

namespace honoka
{
    Epoller::Epoller(Reactor* reactor):reactor_(reactor), cur_fds_num(0){}

    void Epoller::init()
    {
        std::lock_guard lock_(mutex_);
        if((epoll_fd = epoll_create(MAXEPOLL)) == -1)
        {
            perror_and_exit("epoll_create()");
        }
    }

    void Epoller::add_listen(int listen_fd)
    {
        listenning_fds_.insert(listen_fd);
        add_wait(listen_fd);
    }

    void Epoller::del_listen(int listen_fd)
    {
        reactor_->del_wait(listen_fd);
        auto ite = listenning_fds_.find(listen_fd);
        listenning_fds_.erase(ite);
    }

    void set_epoll_ev(struct epoll_event* ev)
    {
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = listen_fd;
    }

    void Epoller::add_wait(int listen_fd)
    {
        struct epoll_event ev;
        set_epoll_ev(ev);

        {
            std::lock_guard lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev ) < 0 )
            {
                perror_and_exit("epoll_add()");
            }
        }

        ++cur_fds_num;
    }

    void Epoller::del_wait(int listen_fd)
    {
        struct epoll_event ev;
        set_epoll_ev(ev);

        {
            std::lock_guard lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, listen_fd, &ev ) < 0 )
            {
                perror_and_exit("epoll_mod()");
            }
        }

        --cur_fds_num;
    }

    void Epoller::run(int delay_time, Thread_pool* thread_pool_, std::map<int, std::shared_ptr<Connection>>&  socket_conns)
    {
        struct epoll_event evs[MAXEPOLL];
        int wait_fds_num;
        {
            std::lock_guard lock_(mutex_);
            if( ( wait_fds_num = epoll_wait( epoll_fd, evs, cur_fds_num, -1 ) ) == -1 )
            {
                perror_and_exit("epoll_wait");
            }
        }

        int conn_fd;
        for (int i = 0; i < wait_fds_num; ++i)
        {
            conn_fd = evs[i].data.fd;
            auto ite =listenning_fds_.find(conn_fd)
            //处理新链接
            if (ite != listenning_fds_.end())
            {
                int newfd;
                while ((newfd = accept(listen_fd,(struct sockaddr *) &servaddr,&len)) > 0)
                {
                    setnonblocking(conn_fd);

                    set_epoll_ev(ev);

                    {
                        std::lock_guard lock_(mutex_);
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd,&ev) == -1)
                        {
                            perror("epoll_ctl: add");
                            exit(EXIT_FAILURE);
                        }
                    }

                    auto tmp_ev = reactor_->create_event(conn_fd, PASSIV_CONN);
                    thread_pool_->add_event(tmp_ev);
                }

                if (conn_fd == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED
                            && errno != EPROTO && errno != EINTR)
                        perror("accept");
                }
                continue;
            }

            if (evs[i].events & EPOLLIN)
            {
                auto tmp_ev = reactor_->create_event(conn_fd, READ_CB);
                thread_pool_->add_event(tmp_ev);
            }
            else if(evs[i].events & EPOLLOUT)
            {
                auto tmp_ev = reactor_->create_event(conn_fd, WRITE_CB);
                thread_pool_->add_event(tmp_ev);
            }
        }
    }

}
