//
// Created by ramizouari on 02/08/2021.
//

#ifndef ALGORITHMICLIBRARY_SEGMENT_TREE_H
#define ALGORITHMICLIBRARY_SEGMENT_TREE_H
#include <vector>
#include <numeric>
#include "binary_operator.h"


template<typename T,typename O=plus_t<T>>
class segment_tree
{
    inline static constexpr O F=O();
    std::vector<T> A;
    std::vector<std::vector<T>> S;
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
            m*=2;
        }
        rec_build(0,0,A.size());
    }
public:
   segment_tree(std::vector<T> &&_A):A(std::move(_A))
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }
    segment_tree(const std::vector<T> &_A):A(_A)
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

template<typename T,typename O=plus_t<T>>
class segment_matrix
{
    using couple=std::pair<int,int>;
    inline static constexpr O F=O();
    std::vector<std::vector<T>> A;
    std::vector<std::vector<std::vector<std::vector<T>>>> S;
    couple dimension;
    couple log_dimension;

    void rec_build(couple depth,couple a,couple L)
    {
        S[depth.first][a.first][depth.second][a.second]=F.neutral;
        if(L.first==1)
        {
            if(L.second==1)
            {
                S[depth.first][a.first][depth.second][a.second]=A[a.first][a.second];
                return;
            }
            else for(int i=0;i<=1;i++)
            {
                rec_build({depth.first,depth.second+1},{a.first,2*a.second+i},{L.first,L.second/2});
                S[depth.first][a.first][depth.second][a.second]=F(S[depth.first][a.first][depth.second][a.second],
                                                                  S[depth.first][a.first][depth.second+1][2*a.second+i]);
            }
        }
        else
        {
            if(L.second==1) for(int i=0;i<=1;i++)
            {
                rec_build({depth.first + 1, depth.second}, {2 * a.first+i, a.second}, {L.first / 2, L.second});
                S[depth.first][a.first][depth.second][a.second] = F(S[depth.first][a.first][depth.second][a.second],
                                                                    S[depth.first + 1][2 *a.first+i][depth.second][a.second]);
            }
            else
            {
                for (int i = 0; i <= 1; i++)
                    rec_build({depth.first + 1, depth.second}, {2 * a.first + i, a.second},
                              {L.first / 2, L.second});
                for (int i = 0; i <= 1; i++)
                    rec_build({depth.first, depth.second+1}, {a.first, 2*a.second+i},
                              {L.first, L.second/2});
                for(int i=0;i<=1;i++) for(int j=0;j<=1;j++)
                        S[depth.first][a.first][depth.second][a.second] = F(S[depth.first][a.first][depth.second][a.second],
                                                                            S[depth.first + 1][2 * a.first + i][
                                                                                    depth.second + 1][2 * a.second + j]);
            }
        }
    }

    void build()
    {
        int n=A.size(),m=1;
        S.clear();
        while(m<=n)
        {
            S.push_back(std::vector<std::vector<std::vector<T>>>(m));
            for(auto &R:S.back())
            {
                int p=n>0?A[0].size():0,q=1;
                while(q<=p)
                {
                    R.push_back(std::vector<T>(q,F.neutral));
                    q*=2;
                }
            }
            m*=2;
        }
        log_dimension={S.size(),S[0][0].size()};
        rec_build({0,0},{0,0},dimension);
    }

public:
    segment_matrix(std::vector<std::vector<T>> &&_A):A(std::move(_A))
    {
        dimension.first=A.size();
        dimension.second=std::accumulate(A.begin(),A.end(),0,[](const auto &U,const auto &V){return std::max<int>(U,V.size());});
        A.resize(std::bit_ceil(A.size()));
        for(auto &R:A)
            R.resize(std::bit_ceil<unsigned int>(dimension.second),F.neutral);
        dimension={A.size(),A[0].size()};
        build();
    }
    segment_matrix(const std::vector<std::vector<T>> &_A):A(_A)
    {
        dimension.first=A.size();
        dimension.second=std::accumulate(A.begin(),A.end(),0,[](const auto &U,const auto &V){return std::max<int>(U,V.size());});
        A.resize(std::bit_ceil(A.size()));
        for(auto &R:A)
            R.resize(std::bit_ceil<unsigned int>(dimension.second),F.neutral);
        dimension={A.size(),A[0].size()};
        build();
    }

    T query(couple l,couple r,couple a,couple b,couple depth) const
    {
        if(l.first==r.first || l.second == r.second)
            return F.neutral;
        auto c=std::make_pair((a.first+b.first)/2,(a.second+b.second)/2);
        if(c.first<l.first)
        {
            c.second=a.second;
            depth.first++;
            return query(l,r,c,b,depth);
        }
        if(c.second<l.second)
        {
            c.first=a.first;
            depth.second++;
            return query(l,r,c,b,depth);
        }
        if(c.first>r.first)
        {
            c.second=b.second;
            depth.first++;
            return query(l,r,a,c,depth);
        }
        if(c.second>r.second)
        {
            c.first=b.first;
            depth.second++;
            return query(l,r,a,c,depth);
        }
        T R=0;
        if(a.first==l.first && r.first==b.first)
        {
            if(a.second==l.second&&r.second==b.second)
                R=S[depth.first][a.first>>(log_dimension.first-depth.first-1)][depth.second][a.second>>(log_dimension.second-depth.second-1)];
            else
            {
                depth.second++;
                R=F(query(l,{r.first,c.second},a,{r.first,c.second},depth),query({l.first,c.second},r,{l.first,c.second},b,depth));
            }
        }
        else
        {
            depth.first++;
            R= F(query(l,{c.first,r.second},a,{c.first,b.second},depth),query({c.first,l.second},r,{c.first,a.second},b,depth));
        }
        return R;
    }

    T query(int l,int r,int p,int q) const
    {
        return query({std::max(l,0),std::max(p,0)},{std::min(r,dimension.first),std::min(q,dimension.second)},{0,0},dimension,{0,0});
    }

    T query(couple L ,couple R) const
    {
        return query(L.first,R.first,L.second,R.second);
    }

};

#endif //ALGORITHMICLIBRARY_SEGMENT_TREE_H