#include "Timing_event.h"


honoka::Timing_event::Timing_event(Timing_type type, int fd):type_(type),fd_(fd){}
honoka::Timing_event::~Timing_event(){}


honoka::Timing::Timing():fd_(-1),value_(0),s_value(0){}
honoka::Timing::Timing(int fd, int value = 10000):fd_(fd), value_(value),s_value(0){}
honoka::Timing::~Timing(){}	
