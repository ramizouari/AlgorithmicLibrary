//
// Created by ramizouari on 05/10/2021.
//

#include <iostream>
#include <vector>
#include "segment_tree.h"
#include "sparse_array.h"
#include "ordered_segment_tree.h"
#include <chrono>
#include <random>
#include "dynamic_segment_tree.h"

#define ll long long
using namespace std;

const int N = 4e5+2; // n max
int n;
ll *a, *tree;

ll mrg(ll x, ll y) {
    return x+y;
}

void build(int id = 0, int ns = 0, int ne = n-1) {
    if (ns == ne) {
        tree[id] = a[ns];
        return;
    }
    int l = 2*id+1;
    int r = 2*id+2;
    int md = (ns+ne)/2;
    build(l, ns, md);
    build(r, md+1, ne);
    tree[id] = mrg(tree[l], tree[r]);
}

ll get(int qs, int qe, int id = 0, int ns = 0, int ne = n-1) {
    if (ns > qe || ne < qs) {
        return 0; // 0 for sum/gcd/or/xor and 1 for and/product and infinity for max/min
    }
    if (qs <= ns && qe >= ne) {
        return tree[id];
    }
    int l = 2*id+1;
    int r = 2*id+2;
    int md = (ns+ne)/2;
    return mrg(get(qs, qe, l, ns, md), get(qs, qe, r, md+1, ne));
}

int main() {
    int Q;
    std::cin >> n >> Q;
    std::random_device dev;
    std::mt19937_64 g(dev());
    std::uniform_int_distribution<ll> d(0,1e9);
    std::uniform_int_distribution<int> d_index(0,n-1);
    a = new ll[n];
    tree = new ll[4*n];
    build();
    std::vector<ll> A(n);
    for (int i = 0; i < n; i++)
    {
        a[i]=d(g);
        A[i]=a[i];
    }
    std::vector<std::pair<ll,ll>> queries(Q);
    for(int i=0;i<Q;i++)
    {
        int a=d_index(g),b=d_index(g);
        auto [l,r]=std::minmax(a,b);
        queries[i]={l,r};
    }
    std::vector<ll> R1(Q),R2(Q);
    auto t1_preconstruct=std::chrono::high_resolution_clock::now();
    build();
    auto t1_postconstruct = std::chrono::high_resolution_clock::now();
    for(int i=0;i<Q;i++)
    {
        auto [l,r]=queries[i];
        R1[i]=get(l,r);
    }
    auto t1_postqueries = std::chrono::high_resolution_clock::now();

    auto t2_preconstruct=std::chrono::high_resolution_clock::now();
    dynamic_segment_tree<ll,plus_t<ll>> T;
    for(auto a:A)
        T.insert(a);
    auto t2_postconstruct = std::chrono::high_resolution_clock::now();
    for(int i=0;i<Q;i++)
    {
        auto [l,r]=queries[i];
        R2[i]=T.query(l,r+1);
    }
    auto t2_postqueries = std::chrono::high_resolution_clock::now();
    ll R=0;
    for(int i=0;i<Q;i++)
        R=std::max<ll>(R,std::abs(R2[i]-R1[i]));
    auto d1=std::chrono::duration_cast<std::chrono::milliseconds>(t1_postqueries-t1_postconstruct).count();
    std::cout << "Segment Tree 1: \nConstruction Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1_postconstruct-t1_preconstruct).count() << "ms";
    std::cout << "\nQueries Time: " << d1 << "ms" << "\nAverage Time per query: " << static_cast<long double>(d1)/Q;
    std::cout << "\n\n" << std::endl;
    auto d2=std::chrono::duration_cast<std::chrono::milliseconds>(t2_postqueries-t2_postconstruct).count();
    std::cout << "Segment Tree 2: \nConstruction Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2_postconstruct-t2_preconstruct).count() << "ms";
    std::cout << "\nQueries Time: " << d2 << "ms" << "\nAverage Time per query: " << static_cast<long double>(d2)/Q;
    std::cout << "\n\nMax Difference: " << R;
    delete[] a;
    delete[] tree;
    return false;
}
