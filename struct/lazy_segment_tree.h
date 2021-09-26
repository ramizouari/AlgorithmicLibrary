//
// Created by ramizouari on 26/09/2021.
//

#ifndef ALGORITHMICLIBRARY_LAZY_SEGMENT_TREE_H
#define ALGORITHMICLIBRARY_LAZY_SEGMENT_TREE_H


#ifndef ALGORITHMICLIBRARY_SEGMENT_TREE_H
#define ALGORITHMICLIBRARY_SEGMENT_TREE_H
#include <vector>
#include <numeric>
#include "binary_operator.h"


template<typename T,typename O=plus_t<T>>
class lazy_segment_tree
{
    inline static constexpr O F=O();
    std::vector<T> A;
    std::vector<std::vector<T>> S;
    std::vector<std::vector<bool>> marked;
    int size;
    void rec_build(int depth,int a,int L)
    {
        S[depth][a]=F.neutral;
        if(L==1)
        {
            S[depth][a]=A[a];
            return;
        }
        if(L>1)
        {
            rec_build(depth+1,2*a,L/2);
            rec_build(depth+1,2*a+1,L/2);
            S[depth][a]=F(S[depth+1][2*a],S[depth+1][2*a+1]);
        }
    }
    void build()
    {
        int n=A.size(),m=1;
        S.clear();
        while(m<=n)
        {
            S.push_back(std::vector<T>(m));
            marked.push_back(std::vector<bool>(m));
            m*=2;
        }
        rec_build(0,0,A.size());
    }
public:
    lazy_segment_tree(std::vector<T> &&_A):A(std::move(_A))
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }
    lazy_segment_tree(const std::vector<T> &_A):A(_A)
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }

    T query(int l,int r,int a,int b,int depth) const
    {
        if(l>=r)
            return F.neutral;
        int c=(a+b)/2;
        if(c<l)
            return query(l,r,c,b,depth+1);
        if(c>r)
            return query(l,r,a,c,depth+1);
        T R;
        if(a==l && r==b)
            R=S[depth][a>>(S.size()-depth-1)];
        else
            R= F(query(l,c,a,c,depth+1),query(c,r,c,b,depth+1));

        return R;
    }
    T query(int l,int r) const
    {
        return query(std::max(l,0),std::min<int>(r,A.size()),0,A.size(),0);
    }

    void update(
            int p,const T &r)
    {
        int m=S.size()-1;
        S[m][p]=r;
        p/=2;
        m--;
        while(m>=0)
        {
            S[m][p]=F(S[m+1][2*p],S[m+1][2*p+1]);
            m--;
            p/=2;
        }
    }

    void update(
            int p,const T &r)
    {
        int m=S.size()-1;
        S[m][p]=r;
        p/=2;
        m--;
        while(m>=0)
        {
            S[m][p]=F(S[m+1][2*p],S[m+1][2*p+1]);
            m--;
            p/=2;
        }
    }

    void update(int l,int r,const T &r)
    {
        int m=S.size()-1;
        S[m][p]=r;
        p/=2;
        m--;
        while(m>=0)
        {
            S[m][p]=F(S[m+1][2*p],S[m+1][2*p+1]);
            m--;
            p/=2;
        }
    }

    void push_back(const T&r)
    {
        if(size==A.size())
        {
            A.resize(2*size);
            A[size]=r;
            build();
        }
        else
        {
            A[size]=r;
            update(size,r);
        }
        size++;
    }

    void pop_back()
    {
        int m=std::bit_ceil<unsigned int>(size-1);
        if(m==size-1)
        {
            size--;
            A.resize(size);
            build();
        }
        else
        {
            update(size-1,F.neutral);
            size--;
        }
    }
};

#endif //ALGORITHMICLIBRARY_LAZY_SEGMENT_TREE_H
