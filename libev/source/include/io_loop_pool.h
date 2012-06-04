#ifndef _IO_LOOP_POOL_H_
#define _IO_LOOP_POOL_H_
#include <vector>
#include "loop.h"
#include <memory>
#include "singleton_base.h"

class io_loop_pool//: public singleton_base<io_loop_pool>
{
    public:
        static io_loop_pool& instance();
        io_loop&   get_io_loop();
        bool  run();
        bool  stop();

    private:
        io_loop_pool(); 
        ~io_loop_pool();

    private:
        std::size_t                              m_size;
        std::size_t                              m_next;
        std::vector<std::shared_ptr<io_loop> >   m_pool;
};

#endif //_IO_LOOP_POOL_H_

