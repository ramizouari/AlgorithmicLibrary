#include "segment_tree.h"
#include "ordered_segment_tree.h"
#include "dynamic_segment_tree.h"
#include <iostream>
#include <vector>

using namespace std;
template<typename T>
struct monoid_t
{
    T operator()(T a,T b) const{
        if(a==0)
            return b;
        return a;
    }
    inline static T neutral =0;
};

int main()
{
    dynamic_segment_tree<int> S;
    for(int i=0;i<10;i++)
        S.insert(i);

    std::cout << S.query(3,6);
    return false;
}