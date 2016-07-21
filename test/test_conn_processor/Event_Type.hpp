#ifndef __EVENT__TYPE__HPP__
#define __EVENT__TYPE__HPP__

namespace honoka
{
    enum Event_Type
    {
        PASSIV_CONN = 0,
        ACTIVE_CONN = 1,
        READ_CB = 2,
        WRITE_CB = 3,
        PASSIVE_CLOSE = 4,
        ACTIVE_CLOSE = 5,
        TIMEOUT = 6
    };
}

#endif
