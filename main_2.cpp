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
    sparse_array<int,gcd_t<int>> S({1,2,3,4,5});
    int m=0;
    std::cout << S.query(2,5);
    return false;
}