#include "Singleton.hpp"
#include "3rd party/json.hpp"

class Configuration : public json,  public Singleton<Configuration>
{
public:
    Configuration();
    ~Configuration();
//应当将其初始化为某个默认json
    void init();
};
