include ../test.make
all : Timing_wheel_unittest

clean :
	rm -f *.o

OBJS = Timing_wheel.o Thread_pool_stub.o Reactor_stub.o Timing_wheel_unittest.o

Timing_wheel.o: ${HONOKA_SRCS}/Timing_wheel.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ${HONOKA_SRCS}/Timing_wheel.cpp

Thread_pool_stub.o: Thread_pool_stub.cpp ${HONOKA_SRCS}/Thread_pool.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Thread_pool_stub.cpp

Reactor_stub.o: Reactor_stub.cpp ${HONOKA_SRCS}/Reactor.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Reactor_stub.cpp

Timing_wheel_unittest.o:Timing_wheel_unittest.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Timing_wheel_unittest.cpp

Timing_wheel_unittest: ${OBJS}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L./glog/lib -lglog -I./glog/include   $^ $(LINKFLAGS) -o  $@ 
