//
// Created by ramizouari on 03/10/2021.
//

#ifndef ALGORITHMICLIBRARY_BINARY_RELATION_H
#define ALGORITHMICLIBRARY_BINARY_RELATION_H

template<typename T>
struct partial_order
{
    virtual bool compare(const &T a,const &T b) const =0;
};


template<typename T>
struct weak_order : public partial_order<T>
{
};

template<typename T>
struct total_order: public weak_order<T>
{
};

template<typename T>
struct less_t:public weak_order<T>
{
    bool compare(const &T a,const &T b) const
    {
        return a<b;
    }

};

template<typename T>
struct greater_t:public weak_order<T>
{
    bool compare(const &T a,const &T b) const
    {
        return a>b;
    }
};

template<typename T>
struct divides_t:public partial_order<T>
{
    bool compare(const &T a,const &T b) const
    {
        return b%a==0;
    }
};
#endif //ALGORITHMICLIBRARY_BINARY_RELATION_H
