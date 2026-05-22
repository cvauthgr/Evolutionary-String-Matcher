#pragma once

#include <random>

namespace random
{

inline std::mt19937 generate()
{
    thread_local std::random_device rd ; //thread_local so each thread get a different seed

    return std::mt19937 ( rd() ) ;
}

inline thread_local std::mt19937  mt { generate() } ; //Can be called from any file ( stand-alone instance ) , thread local for the same reason
//otherwise all threads use the same random value at each respective simualation , mt19937 to avoid repetitiond in random number generations

template < typename T >
T getReal(T min , T max)
{
    return std::uniform_real_distribution<T>{ min , max }(mt) ;
}


int getInt( int min , int max)
{
    return std::uniform_int_distribution{ min , max }(mt) ;
}


}
