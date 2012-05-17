#ifndef _IO_LOOP_POOL_H_
#define _IO_LOOP_POOL_H_
#include "loop.h"
#include "singleton_base.h"

class io_loop_pool: public singleton_base<io_loop_pool>
{
    public:
        io_loop_pool& instance();
        io_loop&   get_io_loop();
        bool  run();
        bool  stop();

    private:
        io_loop_pool(); 
        io_loop_pool(const std::size_t& size);
        ~io_loop_pool();


    private:
        std::size_t  m_size;
        std::vector<io_pool>   m_pool;
};

#endif //_IO_LOOP_POOL_H_

