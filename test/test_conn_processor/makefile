

All: unittest

clean : 
	rm -f *.o

Connection_processor.o : Connection_processor.cpp Connection_processor.h Event_Type.hpp
	g++ -c Connection_processor.cpp -o Connection_processor.o -std=c++11

Conn_processor_unittest.o : Conn_processor_unittest.cpp Event_Type.hpp
	g++ -c Conn_processor_unittest.cpp -o Conn_processor_unittest.o -std=c++11

unittest :Connection_processor.o Conn_processor_unittest.o 
	g++ Connection_processor.o Conn_processor_unittest.o -o unittest -std=c++11

