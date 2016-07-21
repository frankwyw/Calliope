include ../test.make

STUBS = Buffer_stub.o

all : Socket_unittest

clean :
	rm -f *.o



Socket.o: $(HONOKA_SRCS)/Socket.cpp $(HONOKA_SRCS)/Socket.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(HONOKA_SRCS)/Socket.cpp 

Socket_unittest.o : Socket_unittest.cpp $(HONOKA_SRCS)/Socket.h
	$(CXX) $(CPPFALGS) $(CXXFLAGS) -c Socket_unittest.cpp 

Buffer_stub.o : Buffer_stub.cpp $(HONOKA_SRCS)/Buffer.h
	$(CXX) $(CPPFALGS) $(CXXFLAGS) -c Buffer_stub.cpp 

Socket_unittest: Socket.o Socket_unittest.o $(STUBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -L./glog/lib -lglog -I./glog/include   $^ $(LINKFLAGS) -o $@ 



