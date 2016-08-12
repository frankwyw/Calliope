include ../test.make

all : Configuration_unittest

clean :
	rm -f *.o

Configuration.o: $(HONOKA_SRCS)/Configuration.cpp $(HONOKA_INCLUDES)/Configuration.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(HONOKA_SRCS)/Configuration.cpp 

Configuration_unittest.o : Configuration_unittest.cpp $(HONOKA_INCLUDES)/Configuration.h
	$(CXX) $(CPPFALGS) $(CXXFLAGS) -c Configuration_unittest.cpp 

Configuration_unittest: Configuration.o Configuration_unittest.o 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -L./glog/lib -lglog -I./glog/include   $^ $(LINKFLAGS) -o $@ 



