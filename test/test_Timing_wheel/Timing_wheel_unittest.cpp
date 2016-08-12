#include "Timing_wheel.h"
#include "Thread_pool.h"
#include "Reactor.h"
#include "gtest/gtest.h"

using namespace honoka;

TEST(Timing, Con)
{
	Reactor* reac_ = nullptr;
	Thread_pool* pool_= nullptr;
	Timing_wheel test(pool_,reac_);
}

TEST(Timing, add_event)
{
	Reactor* reac_ = nullptr;
	Thread_pool* pool_= nullptr;
	Timing_wheel test(pool_,reac_);
	Timing_event t(NEW_TIMING,100);
	test.add_timing_event(t);
	sleep(1);	
}

TEST(Timing, runNEW_TIMING)
{
	Reactor* reac_ = nullptr;
	Thread_pool* pool_= nullptr;
	Timing_wheel test(pool_,reac_);
	Timing_event t1(NEW_TIMING, 15);
	Timing_event t2(NEW_TIMING, 21);
	Timing_event t3(NEW_TIMING, 20);
	Timing_event t4(NEW_TIMING, 30);

	test.add_timing_event(t1);
	test.add_timing_event(t2);


	sleep(20);
	
	test.add_timing_event(t3);
	test.add_timing_event(t4);
	
	sleep(20);
	
}

TEST(Timing, runUPDATE_TIMING)
{
	Reactor* reac_ = nullptr;
	Thread_pool* pool_= nullptr;
	Timing_wheel test(pool_,reac_);
	Timing_event t1(NEW_TIMING, 15);
	Timing_event t2(NEW_TIMING, 21);
	Timing_event t3(NEW_TIMING, 20);
	Timing_event t4(NEW_TIMING, 30);

	test.add_timing_event(t1);
	test.add_timing_event(t2);	
	test.add_timing_event(t3);
	test.add_timing_event(t4);

	sleep(5);

	Timing_event t5(UPDATE_TIMING, 30);
	test.add_timing_event(t5);
	
	sleep(20);	
}

TEST(Timing, runDEL_TIMING)
{
	Reactor* reac_ = nullptr;
	Thread_pool* pool_= nullptr;
	Timing_wheel test(pool_,reac_);
	Timing_event t1(NEW_TIMING, 15);
	Timing_event t2(NEW_TIMING, 21);
	Timing_event t3(NEW_TIMING, 20);
	Timing_event t4(NEW_TIMING, 30);

	test.add_timing_event(t1);
	test.add_timing_event(t2);	
	test.add_timing_event(t3);
	test.add_timing_event(t4);

	sleep(5);

	Timing_event t5(DEL_TIMING, 30);
	test.add_timing_event(t5);
	
	sleep(20);	
}



