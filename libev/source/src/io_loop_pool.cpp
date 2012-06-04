#include "io_loop_pool.h"
#include "configuration.h"

io_loop_pool& io_loop_pool::instance()
{
    static io_loop_pool  pool;
    return pool;
}

io_loop&   io_loop_pool::get_io_loop()
{
    if (m_next < m_size)
    {
        __sync_add_and_fetch(&m_next, 1);
    }
    else
    {
        m_next = 0;
    }

    return *m_pool[m_next];
}

bool  io_loop_pool::run()
{
    for (std::size_t i = 0; i < m_size; ++i)
    {
        m_pool[i]->run();
    }
    return true;
}

bool  io_loop_pool::stop()
{
    for (std::size_t i = 0; i < m_size; ++i)
    {
        m_pool[i]->stop();
    }
    return true;
}

io_loop_pool::io_loop_pool(): m_size(configuration::instance().get_ev_loop_size()), m_next(-1)
{
    for (std::size_t i = 0; i < m_size; ++i)
    {
        m_pool.push_back(std::shared_ptr<io_loop>(new (std::nothrow) io_loop()));
    }
}

io_loop_pool::~io_loop_pool()
{
    stop();
}

