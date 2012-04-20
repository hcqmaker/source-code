#ifndef LOOP_H_
#define LOOP_H_

class io_loop
{
    public:
        io_loop();
        io_loop(const unsigned size, const std::size_t*  array);
        ~io_loop();

    private:
        //make it can not be copied
        io_loop(const io_loop&);
        io_loop operator=(const io_loop&);

    private:
        struct ev_loop* loop;
};

#endif //LOOP_H_
