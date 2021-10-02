#ifndef ___BINARY_OPERATOR___
#define ___BINARY_OPERATOR___

template<typename T>
struct group_operation
{
};

template<typename T>
struct idempotent_operation
{

};

template<typename T>
struct plus_t : public group_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return a+b;
    }
    inline static T neutral=T();
    T inv(const T&a) const
    {
        return -a;
    }
};


template<typename T>
struct max_t:public idempotent_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return std::max(a,b);
    }
    inline static T neutral=T();
};

template<typename T>
struct min_t:public idempotent_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return std::min(a,b);
    }
    inline static T neutral=T();
};


template<typename T>
struct multiplies_t
{
    T operator()(const T&a,const T&b) const
    {
        return a*b;
    }
    inline static T neutral=T(1);
};

template<typename T>
struct gcd_t:public idempotent_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return std::gcd(a,b);
    }
    inline static T neutral=T();
};


template<typename T>
struct lcm_t:public idempotent_operation<T>
        {
    T operator()(const T&a,const T&b) const
    {
        return std::lcm(a,b);
    }
    inline static T neutral=T(1);
        };

template<typename T>
struct xor_t:public group_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return a^b;
    }
    inline static T neutral=T();
    T inv(const T&a) const{
        return a;
    }
};

template<typename T>
struct and_t:public idempotent_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return a&b;
    }
    inline static T neutral=T(-1);
};

template<typename T>
struct or_t: public idempotent_operation<T>
{
    T operator()(const T&a,const T&b) const
    {
        return a|b;
    }
    inline static T neutral=T();
};

#endif