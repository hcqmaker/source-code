#include "loop.h"

io_loop::io_loop()
{
    loop = ev_loop_new(EVFLAG_AUTO);
}

io_loop::io_loop(const unsigned size, const std::size_t*  array);
{
    std::size_t flag = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        flag = flag | array[i];
    }

    loop = ev_loop_new(flag);
}

io_loop::~io_loop()
{
    ev_loop_destroy(loop);
}

bool io_loop::run()
{
    return ev_run(loop, 0);
}

bool io_loop::stop()
{
    return ev_break(loop, EVBREAK_ALL);
}

bool io_service::add_service(const ev_io*  service)
{
    ev_io_start(loop, service);
    return true;
}

bool io_service::del_service(const ev_io*  service)
{
    ev_io_start(loop, service);
    return true;
}

