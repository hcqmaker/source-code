//here is a singleton object, and it will constructed in _start function 
//it's thread safe as it constructed in singleton thread before main

#ifndef SINGLETON_BASE_H_
#define SINGLETON_BASE_H_

template<typename _T>
class  singleton_base
{
private:
    struct _object_create
    {
        _object_create()
        {
            singleton_base<_T>::instance();
        }
        
        inline void do_nothing()
        {
        }
    };

static _object_create  obj;

private:
    singleton_base()
    {
    }

public:

    static _T&  instance()
    {
        static _T  singleton;

        singleton.do_nothing();

        return singleton;
    }
};

template<typename _T>
typename singleton_base<_T>::_object_create singleton_base<_T>::obj;

#endif //SINGLETON_BASE_H_
