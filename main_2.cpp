#include "segment_tree.h"
#include "sparse_array.h"
#include "dynamic_segment_tree.h"
#include "ordered_segment_tree.h"
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
    ordered_segment_tree<int> S;
    int m=0;
    for(int i=0;i<5;i++)
        S.insert(i);
    S.remove(2);
    std::cout << m;
    return false;
}