include ../test.make

all : Thread_pool_unittest

clean :
	rm -f *.o

Thread_pool.o: $(HONOKA_SRCS)/Thread_pool.cpp $(HONOKA_SRCS)/Thread_pool.h $(HONOKA_SRCS)/Event.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(HONOKA_SRCS)/Thread_pool.cpp 

Thread_pool_unittest.o : Thread_pool_unittest.cpp $(HONOKA_SRCS)/Thread_pool.h Event.h
	$(CXX) $(CPPFALGS) $(CXXFLAGS) -c Thread_pool_unittest.cpp 

Event.o: Event.cpp Event.h
	$(CXX) $(CPPFALGS) $(CXXFLAGS) -c Event.cpp

Thread_pool_unittest: Thread_pool.o Thread_pool_unittest.o Event.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -L./glog/lib -lglog -I./glog/include   $^ $(LINKFLAGS) -o $@ 



