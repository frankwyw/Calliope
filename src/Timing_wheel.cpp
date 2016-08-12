
#include "Timing_wheel.h"

#include <unistd.h>


#include "Reactor.h"
#include "Thread_pool.h"
#include "Event_Type.hpp"
#include "Timing_Event_Type.hpp"
#include "Timing_event.h"

#include <glog/logging.h>

//#include <iostream>

#include <chrono>

#define WAIT_TIME (std::chrono::system_clock::now() + std::chrono::milliseconds(20))

#define S_TO_MS 1000

#define MSVEC_MAX 50
#define MSVEC_TIME 20
#define SVEC_MAX 18

#define ITE_INIT 0
#define VALID_MIN_TIME 0
#define INVALID_MAX_TIME -1
#define INVALID_FD -1
#define NO_TIME 0
#define INIT 0
#define DEFAULT_VEC 10
#define MORE_FD_SIZE  ((fd)+(2))

#define DEfAULT_INIT_TIME 10000




honoka::Timing::Timing():fd_(-1),value_(0),s_value(0){}
honoka::Timing::Timing(int fd, int value = DEfAULT_INIT_TIME):fd_(fd), value_(value),s_value(0){}
honoka::Timing::~Timing(){}	




honoka::Timing_wheel::Timing_wheel(Thread_pool* thread_pool, Reactor* reactor):queue_(),mutex_(),ms_vec(MSVEC_MAX),s_vec(SVEC_MAX),
	vec_(),cur_ms(std::begin(ms_vec)),cur_s(std::begin(s_vec)),cur_ms_num(INIT),cur_s_num(INIT),cur(std::begin(*cur_ms)),
	thread_pool_(thread_pool), reactor_(reactor),is_stop(false),
	thread_([this](){
		for(;;)
	{
		{
			std::unique_lock<std::mutex> lk(mutex_);
			this->cv.wait_until(lk, WAIT_TIME);
			if(this->is_stop)
				return;
			while(!this->queue_.empty())
			{
				auto tmp_event = this->queue_.front();
				this->queue_.pop();
				switch(tmp_event.type_)
				{
					case NEW_TIMING:
						this->create_timing(tmp_event.fd_);
						LOG(INFO)<<"fd new timing";
						break;
					case UPDATE_TIMING:
						this->update_timing(tmp_event.fd_);
						LOG(INFO)<<"fd update timing";
						break;
					case DEL_TIMING:
						this->del_timing(tmp_event.fd_);
						LOG(INFO)<<"fd del timing";
						break;
					default:
						LOG(FATAL)<<"Timing_wheel unknown timing type";
						break;
				}
			}		
		}

		int fd;
		cur = std::begin(*cur_ms);
		cur_ed = std::end(*cur_ms);
		skip_within_time();
		while((fd = get_timeout()) > 0)
		{
			//std::cout<<fd<<"aa"<<std::endl;
			if(reactor_ == nullptr)
			{
				LOG(ERROR)<<"Timing_wheel::Timing_wheel() reactor_ == nullptr";
			}
			if(thread_pool_ == nullptr)
			{
				LOG(ERROR)<<"Timing_wheel::Timing_wheel() thread_pool_ == nullptr";
			}
			auto tmp_event = reactor_->create_event(fd, TIMEOUT);
			thread_pool_->add_event(tmp_event);
			++cur;
			skip_within_time();
		}
		cur_ms->clear();
		advance();
	}	
	})

{
	//std::vector<int> tmp_vec;
	

}

honoka::Timing_wheel::~Timing_wheel()
{
	stop();
	cv.notify_all();
	thread_.join();
}

void honoka::Timing_wheel::stop()
{
	std::lock_guard<std::mutex> g(mutex_);
	is_stop = true;
}

void honoka::Timing_wheel::add_timing_event(const Timing_event& event)
{
	std::lock_guard<std::mutex> g(mutex_);
	queue_.push(event);
	cv.notify_one();
}

void honoka::Timing_wheel::run()
{
	for(;;)
	{
		{
			std::lock_guard<std::mutex> g(mutex_);
			if(is_stop)
				break;
			while(!queue_.empty())
			{
				auto tmp_event = queue_.front();
				queue_.pop();
				switch(tmp_event.type_)
				{
					case NEW_TIMING:
						create_timing(tmp_event.fd_);
						break;
					case UPDATE_TIMING:
						update_timing(tmp_event.fd_);
						break;
					case DEL_TIMING:
						del_timing(tmp_event.fd_);
						break;
					default:
						break;
				}
			}		
		}

		int fd;
		cur = std::begin(*cur_ms);
		cur_ed = std::end(*cur_ms);
		skip_within_time();
		while((fd = get_timeout()) != 0)
		{

			if(reactor_ == nullptr)
			{
				LOG(ERROR)<<"Timing_wheel::Timing_wheel() reactor_ == nullptr";
			}
			if(thread_pool_ == nullptr)
			{
				LOG(ERROR)<<"Timing_wheel::Timing_wheel() thread_pool_ == nullptr";
			}

			auto tmp_event = reactor_->create_event(fd, TIMEOUT);
			thread_pool_->add_event(tmp_event);
			skip_within_time();
		}
		cur_ms->clear();
		advance();
	}
}

void honoka::Timing_wheel::create_timing(int fd)
{
	int size = vec_.size();
	if(size < fd)
	{	
		vec_.resize(MORE_FD_SIZE);
	}
	Timing tmp_tim(fd);
	install_new_timing(tmp_tim);
	vec_[fd] = tmp_tim;
}

void honoka::Timing_wheel::update_timing(int fd)
{
	vec_[fd].value_ = DEfAULT_INIT_TIME;
	install_new_timing(vec_[fd]);
}


void honoka::Timing_wheel::del_timing(int fd)
{
	vec_[fd].fd_ = INVALID_FD;
	vec_[fd].value_ = INVALID_MAX_TIME;
}

void honoka::Timing_wheel::install_new_timing(Timing& tim)
{
	int val = tim.value_;
	int now_val = cur_ms_num * MSVEC_TIME;
	std::vector<std::vector<int>>::iterator pos = cur_ms;
	int sum_value = val + now_val;
	int s_value = cur_s_num;
	if(sum_value <= S_TO_MS)
	{
		pos += val/20;
		tim.value_ = VALID_MIN_TIME;
	}
	else
	{
		int next_s = sum_value / S_TO_MS;
		tim.value_ = tim.value_ - next_s * S_TO_MS + now_val;
		s_value = (next_s + cur_s_num) % 18;
		pos = std::begin(s_vec) + s_value;
	}
	tim.s_value = s_value;
	pos->push_back(tim.fd_);
}

int honoka::Timing_wheel::get_timeout()
{
	if(cur < cur_ed && vec_[*cur].value_ == NO_TIME && vec_[*cur].s_value == cur_s_num)
		return *cur;
	return 0;
}

void honoka::Timing_wheel::skip_within_time()
{
	for(;cur < cur_ed;cur++)
	{
		if(vec_[*cur].value_ == NO_TIME)
			break;
	}
}

void honoka::Timing_wheel::install_s_timing_to_ms()
{
	//int len = cur_s->size();
	cur = std::begin(*cur_s);
	cur_ed = std::end(*cur_s);
	for(; cur < cur_ed; ++cur )
	{
		if(vec_[*cur].s_value == cur_s_num)
			install_new_timing(vec_[*cur]);
	}
	cur_s->clear();
}


void honoka::Timing_wheel::advance()
{
	++cur_ms_num;
	if(cur_ms_num >= MSVEC_MAX)
	{
		cur_ms_num = ITE_INIT;
		++cur_s_num;
		if(cur_s_num >= SVEC_MAX)
			cur_s_num = ITE_INIT;
		
		cur_s = std::begin(s_vec)+ cur_s_num;
		cur_ms = std::begin(ms_vec)+cur_ms_num;
		install_s_timing_to_ms();
	}
	else
	{
		cur_s = std::begin(s_vec)+ cur_s_num;
		cur_ms = std::begin(ms_vec)+cur_ms_num;
	}

	
	
}




