#include "Configuration.h"
#include <sstream>
#include <fstream>

namespace honoka
{
Configuration::Configuration(){}
Configuration::~Configuration(){}

void Configuration::init()
{
    

    std::string text;
    std::stringstream ss(text);

    ss>>*this;
}

void Configuration::init(std::string filename){}
void Configuration::init(char* filename){}
}
