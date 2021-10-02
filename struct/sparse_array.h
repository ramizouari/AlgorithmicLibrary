//
// Created by ramizouari on 30/09/2021.
//

#ifndef ALGORITHMICLIBRARY_SPARSE_ARRAY_H
#define ALGORITHMICLIBRARY_SPARSE_ARRAY_H
#include "binary_operator.h"
#include <queue>

template<typename T,typename O>
class sparse_array;

template<typename T,typename O> requires (std::is_base_of_v<group_operation<T>,O>)
class sparse_array<T,O>
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
            rec_build(depth+1,2*a+L/2,L/2);
            S[depth][a]=F(S[depth+1][2*a],S[depth+1][2*a+L/2]);
        }
    }
    void build()
    {
        int n=A.size(),m=1;
        S.clear();
        while(m<=n)
        {
            S.push_back(std::vector<T>(n-n/m+1));
            m*=2;
        }
        using couple = std::pair<int,int>;
        std::vector<std::vector<bool>> visited(S.size(),std::vector<bool>(n,false));
        std::queue<couple> Q;
        for(int i=0;i<n;i++) {
            Q.push({S.size() - 1, i});
            visited[S.size()-1][i]=true;
        }
        while(!Q.empty())
        {
            auto [d,a]=Q.front();
            Q.pop();
            if(d==S.size()-1)
                S[d][a]=A[a];
            else
                S[d][a]=F(S[d+1][a],S[d+1][a+(n>>(d+1))]);
            auto size=n-(n>>(d-1))+1;
            if(d>0 && a < size && !visited[d-1][a])
            {
                visited[d-1][a]=true;
                Q.push({d-1,a});
            }
        }
    }
public:
    sparse_array(std::vector<T> &&_A):A(std::move(_A))
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }
    sparse_array(const std::vector<T> &_A):A(_A)
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }

    T query(int l,int r) const
    {
        if(r<=l)
            return O::neutral;
        if(r>A.size())
            return query(l,A.size());
        if(l<0)
            return query(0,r);
        unsigned int b=std::bit_width<unsigned int>(r-l)-1,s=S.size()-b-1;
        if(r-(1<<b)==l)
            return S[s][l];
        auto collision=query(r-(1<<b),l+(1<<b));
        return F(F(S[s][l],S[s][r-(1<<b)]),F.inv(collision));

    }
};

template<typename T, typename O> requires (std::is_base_of_v<idempotent_operation<T>,O>)
class sparse_array<T,O>
{
    inline static constexpr O F=O();
    std::vector<T> A;
    std::vector<std::vector<T>> S;
    int size;

    void build()
    {
        int n=A.size(),m=1;
        S.clear();
        while(m<=n)
        {
            S.push_back(std::vector<T>(n-n/m+1));
            m*=2;
        }
        using couple = std::pair<int,int>;
        std::vector<std::vector<bool>> visited(S.size(),std::vector<bool>(n,false));
        std::queue<couple> Q;
        for(int i=0;i<n;i++) {
            Q.push({S.size() - 1, i});
            visited[S.size()-1][i]=true;
        }
        while(!Q.empty())
        {
            auto [d,a]=Q.front();
            Q.pop();
            if(d==S.size()-1)
                S[d][a]=A[a];
            else
                S[d][a]=F(S[d+1][a],S[d+1][a+(n>>(d+1))]);
            auto size=n-(n>>(d-1))+1;
            if(d>0 && a < size && !visited[d-1][a])
            {
                visited[d-1][a]=true;
                Q.push({d-1,a});
            }
        }
    }
public:
    sparse_array(std::vector<T> &&_A):A(std::move(_A))
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }
    sparse_array(const std::vector<T> &_A):A(_A)
    {
        size=A.size();
        A.resize(std::bit_ceil(A.size()),F.neutral);
        build();
    }

    T query(int l,int r) const
    {
        if(r<=l)
            return O::neutral;
        if(r>A.size())
            return query(l,A.size());
        if(l<0)
            return query(0,r);
        unsigned int b=std::bit_width<unsigned int>(r-l)-1,s=S.size()-b-1;
        return F(S[s][l],S[s][r-(1<<b)]);

    }
};

#endif //ALGORITHMICLIBRARY_SPARSE_ARRAY_H
