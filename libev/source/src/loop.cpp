#include "loop.h"

io_loop::io_loop()
{
    m_loop = ev_loop_new(EVFLAG_AUTO);
}

io_loop::io_loop(const unsigned size, const size_t*  arr)
{
    size_t flag = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        flag = flag | arr[i];
    }

    m_loop = ev_loop_new(flag);
}

io_loop::~io_loop()
{
    ev_loop_destroy(m_loop);
}

bool io_loop::run()
{
    ev_run(m_loop, 0);
    return true;
}

bool io_loop::stop()
{
    ev_break(m_loop, EVBREAK_ALL);
    return true;
}

bool io_loop::add_service(ev_io*  svc)
{
    ev_io_start(m_loop, svc);
    return true;
}

bool io_loop::del_service(ev_io*  service)
{
    ev_io_start(m_loop, service);
    return true;
}

