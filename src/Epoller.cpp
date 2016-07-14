#define MAXEPOLL 1024

#include "Epoller.h"

#include <memory>
#include <sys/epoll.h>

#include <glog/logging.h>

#include "Event.h"
#include "Reactor.h"
#include "tool_function.hpp"
#include "Event_Type.hpp"

namespace honoka
{
    Epoller::Epoller(Reactor* reactor):reactor_(reactor), cur_fds_num(0){}

    void Epoller::init()
    {
        std::lock_guard<std::mutex> lock_(mutex_);
        if((epoll_fd = epoll_create(MAXEPOLL)) == -1)
        {
            LOG(ERROR)<<"Epoller::init() epoll_create() fail";
        }
    }

    void Epoller::add_listen(std::shared_ptr<Socket> socket)
    {
        listenning_fds_.insert(socket->get_fd());
        add_wait(socket);
    }

    void Epoller::del_listen(std::shared_ptr<Socket> socket)
    {
        reactor_->del_wait(socket);
        auto ite = listenning_fds_.find(socket->get_fd());
        listenning_fds_.erase(ite);
    }

    void set_epoll_ev(struct epoll_event* ev)
    {
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = listen_fd;
    }

    void Epoller::add_wait(std::shared_ptr<Socket> socket)
    {
        struct epoll_event ev;
        set_epoll_ev(ev);

        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket->get_fd(), &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::add_wait() epoll_add() fail";
            }
        }

        ++cur_fds_num;
    }

    void Epoller::del_wait(std::shared_ptr<Socket> socket)
    {
        struct epoll_event ev;
        set_epoll_ev(ev);

        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socket->get_fd(), &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::del_wait() epoll_mod() fail";
            }
        }

        --cur_fds_num;
    }

    void Epoller::close_listenning()
    {
        struct epoll_event ev;
        set_epoll_ev(ev);

        std::lock_guard<std::mutex> lock_(mutex_);
        for(auto i : listenning_fds_)
        {
            if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, i, &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::close_listenning() epoll_mod() fail";
            }
        }
    }

    void Epoller::run(int delay_time, Thread_pool* thread_pool_, std::map<int, std::shared_ptr<Connection>>&  socket_conns)
    {
        struct epoll_event evs[MAXEPOLL];
        int wait_fds_num;
        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if( ( wait_fds_num = epoll_wait( epoll_fd, evs, cur_fds_num, -1 ) ) == -1 )
            {
                LOG(ERROR)<<"Epoller::run() epoll_wait() fail";

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
                struct sockaddr_in servaddr;
                bzero(&servaddr, sizeof(struct sockaddr_in));
                int len;
                while ((newfd = accept(listen_fd,(struct sockaddr *) &servaddr,&len)) > 0)
                {
                    setnonblocking(conn_fd);

                    set_epoll_ev(ev);

                    {
                        std::lock_guard lock_(mutex_);
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd,&ev) == -1)
                        {
                            LOG(ERROR)<<"Epoller::run() epoll_add() fail";
                            exit(EXIT_FAILURE);
                        }
                    }
                    auto tmp_socket = std::make_shared<Socket>(newfd, servaddr, len);
                    auto tmp_ev = reactor_->create_new_conn_event(tmp_socket, PASSIV_CONN);
                    thread_pool_->add_event(tmp_ev);
                }

                if (conn_fd == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED
                            && errno != EPROTO && errno != EINTR)
                            LOG(ERROR)<<"Epoller::run() accpet() fail";
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
