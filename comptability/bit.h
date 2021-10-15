//
// Created by ramizouari on 15/10/2021.
//

#ifndef ALGORITHMICLIBRARY_BIT_H
#define ALGORITHMICLIBRARY_BIT_H
#include <numeric>

template<typename T>
unsigned short bit_width(T a)
{
    constexpr unsigned short L=sizeof(T)*8;
    if(a==0)
        return 0;
    unsigned short a=0,b=L,r=0,c;
    while(a<=b)
    {
        c=(a+b)/2;
        if(a>>c)
        {
            r=c;
            a=c+1;
        }
        else b=c-1;
    }
    return r+1;
}

template<typename T>
T bit_ceil(T a)
{
    return a<<(1+bit_width(a));
}

template<typename T>
T bit_floor(T a)
{
    return a<<bit_width(a);
}

#endif //ALGORITHMICLIBRARY_BIT_H
