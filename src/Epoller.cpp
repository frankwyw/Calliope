#define MAXEPOLL 1024

#include "Epoller.h"

#include <memory>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>

#include <glog/logging.h>

#include "Event.h"
#include "Reactor.h"
#include "tool_function.h"
#include "Event_Type.hpp"
#include "Socket.h"
#include "Thread_pool.h"
#include "Connection.h"

namespace honoka
{
    Epoller::Epoller(Reactor* reactor, std::map<int, std::shared_ptr<Connection>>* fd_sockets_conns):reactor_(reactor)
	,cur_fds_num(0),listenning_fds_(), mutex_(), fd_sockets_conns_(fd_sockets_conns){}

    void Epoller::init()
    {
        DLOG(INFO)<<"Epoller::init()";
        std::lock_guard<std::mutex> lock_(mutex_);
        if((epoll_fd = epoll_create(MAXEPOLL)) == -1)
        {
            LOG(ERROR)<<"Epoller::init() epoll_create() fail";
        }
    }

    void Epoller::add_listen(std::shared_ptr<Socket> socket)
    {
        DLOG(INFO)<<"Epoller::add_listen(std::shared_ptr<Socket>)";
        int fd = socket->get_fd();

        {
            std::lock_guard<std::mutex> lock_(mutex_);
            listenning_fds_.insert(fd);
        }


        struct ::epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP;
        ev.data.fd = fd;

        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket->get_fd(), &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::add_wait() epoll_add() fail";
            }
        }

        ++cur_fds_num;
    }

    void Epoller::del_listen(std::shared_ptr<Socket> socket)
    {
        DLOG(INFO)<<"Epoller::del_listen(std::shared_ptr<Socket>)";
        std::lock_guard<std::mutex> lock_(mutex_);
        auto ite = listenning_fds_.find(socket->get_fd());
        listenning_fds_.erase(ite);
        del_wait(socket->get_fd());
    }

    void Epoller::set_epoll_ev(struct ::epoll_event* ev, int fd)
    {
        ev->events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        ev->data.fd = fd;
    }

    void Epoller::add_wait(int fd)
    {
        DLOG(INFO)<<"Epoller::add_wait(std::shared_ptr<Socket>)";
        struct ::epoll_event ev;
        set_epoll_ev(&ev, fd);

        {
            std::lock_guard<std::mutex> lock_(mutex_);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::add_wait() epoll_add() fail";
            }
	    ++cur_fds_num;
        }

    }

    void Epoller::del_wait(int fd)
    {
        DLOG(INFO)<<"Epoller::del_wait(std::shared_ptr<Socket>)";
        struct ::epoll_event ev;

        set_epoll_ev(&ev, fd);

        {
            std::lock_guard<std::mutex> lock_(mutex_);
	          auto ite = fd_sockets_conns_->find(fd);
            if(ite == fd_sockets_conns_->end())
	          {
		            LOG(ERROR)<<"del_wait fd not exist";
		            return;
	          }


            if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::del_wait() epoll_del() fail";
            }
            fd_sockets_conns_->erase(ite);
	          --cur_fds_num;
        }
    }

    void Epoller::close_listenning()
    {
        DLOG(INFO)<<"Epoller::close_listenning()";
        struct ::epoll_event ev;

        std::lock_guard<std::mutex> lock_(mutex_);
        for(auto i : listenning_fds_)
        {
            set_epoll_ev(&ev, i);
            if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, i, &ev ) < 0 )
            {
                LOG(ERROR)<<"Epoller::close_listenning() epoll_mod() fail";
            }
        }
    }

    void Epoller::run(int delay_time, Thread_pool* thread_pool_)
    {
//	DLOG(INFO)<<"Epoller::run(int. thread_pool*, std::map<...>)";
        struct ::epoll_event evs[MAXEPOLL];
        int wait_fds_num;
        {
	    std::unique_lock<std::mutex> lock_(mutex_);
//	        if(create_event_num != 0 || del_event_num != 0)
//	    	cond_var.wait(lock_,[this]{ return this->create_event_num == 0 && del_event_num == 0; });
            if( ( wait_fds_num = epoll_wait( epoll_fd, evs, cur_fds_num, -1 ) ) == -1 )
            {
//                LOG(ERROR)<<"Epoller::run() epoll_wait() fail";
//                DLOG(FATAL)<<"BUG";
            }
        }

        int conn_fd;
        for (int i = 0; i < wait_fds_num; ++i)
        {
            conn_fd = evs[i].data.fd;
            auto ite =listenning_fds_.find(conn_fd);
            //处理新链接
            if (ite != listenning_fds_.end())
            {
                int new_fd;
                auto servaddr = std::make_shared<struct sockaddr_in>();
                bzero(servaddr.get(), sizeof(struct sockaddr_in));
                socklen_t len;
                while ((new_fd = ::accept(conn_fd,(struct sockaddr *) servaddr.get(),&len)) > 0)
                {
                    setnonblocking(new_fd);
                    add_wait(new_fd);
                    auto tmp_socket = std::make_shared<Socket>(new_fd, servaddr, len);

                    auto tmp_ev = reactor_->create_new_conn_event(tmp_socket, PASSIV_CONN);


                    thread_pool_->add_event(tmp_ev);
                }

                if (conn_fd == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
                            LOG(ERROR)<<"Epoller::run() accpet() fail";
                }
                continue;
            }

            if(evs[i].events & EPOLLRDHUP)
	        {
                char buf[2];
                ::read(conn_fd, buf, 0);
                del_wait(conn_fd);
                auto tmp_ev = reactor_->create_event(conn_fd, PASSIVE_CLOSE);
                thread_pool_->add_event(tmp_ev);
	        }
            else if (evs[i].events & EPOLLIN)
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
