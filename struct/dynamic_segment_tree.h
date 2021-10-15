//
// Created by ramizouari on 26/09/2021.
//
/*
 * A Dynamic Segment Tree of size n is a Data Structure supporting:
 * 1. Insertion and Deletion Anywhere in O(ln n)
 * 2. Range Queries on  O(ln n)
 * 3. O(n) memory
 *
 * The Construction time will take O(n*ln n)
 * */


#ifndef ALGORITHMICLIBRARY_dynamic_segment_tree_H
#define ALGORITHMICLIBRARY_dynamic_segment_tree_H
#include "binary_operator.h"


template<typename T,typename O =plus_t<T>>
struct dst_node
{
    inline static O F=O();
    T v,sum;
    int height,size;
    dst_node * left, * right, *parent;
};

namespace dst_utils
{

    template<typename T, typename O=plus_t<T>>
    inline int height(dst_node<T,O>* tree)
    {
        if (!tree)
            return 0;
        return tree->height;
    }

    template<typename T, typename O=plus_t<T>>
    inline int size(dst_node<T,O>* tree)
    {
        return tree?tree->size:0;
    }


    template<typename T, typename O=plus_t<T>>
    bool find(dst_node<T,O>* a, T v)
    {
        if (!a)
            return false;
        if (v == a->v)
            return true;
        else if (v > a->v)
            return find(a->right, v);
        else return find(a->left, v);
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* find_by_order(dst_node<T,O>* a, int order)
    {
        if(!a)
            return nullptr;
        if(size(a->left)==order)
            return a;
        else if(size(a->left)>order)
            return find_by_order(a->left,order);
        else return find_by_order(a->right,order-size(a->left)-1);
    }

    template<typename T, typename O=plus_t<T>>
    int order(dst_node<T,O>* a, T v,int offset=0)
    {
        if (!a)
            return -1;
        if (v == a->v)
            return offset+(a?size(a->left):0);
        else if (v > a->v)
            return order(a->right, v,offset+1+(a?size(a->left):0));
        else return order(a->left, v,offset);
    }

    template<typename T, typename O=plus_t<T>>
    T prefix_query(dst_node<T,O>* a, int n)
    {
        using K=dst_node<T,O>;
        if (!a || n<=0)
            return O::neutral;
        else if(n<size(a->left))
            return prefix_query(a->left,n);
        else if(n==size(a->left))
            return a->left?a->left->sum:O::neutral;
        else if(n==a->size)
            return a->sum;
        return K::F(K::F(a->left?a->left->sum:O::neutral,a->v),prefix_query(a->right,n-size(a->left)-1));
    }

    template<typename T, typename O=plus_t<T>>
    T query(dst_node<T,O>* a, int l,int r)
    {
        using K=dst_node<T,O>;
        if(!a || l>=r || r<=0)
            return O::neutral;
        if(l==0)
            return prefix_query(a,r);

        if(size(a->left)>=l)
        {
            T R1=query(a->left,l,std::min(size(a->left),r)),R2=query(a->right,0,std::max(r-size(a->left)-1,0));
            T S=O::neutral;
            if(r>size(a->left))
                S=a->v;
            return K::F(K::F(R1,S),R2);
        }
        else return query(a->right,l-size(a->left)-1,std::max(r-size(a->left)-1,0));
    }

    template<typename T, typename O=plus_t<T>>
    T sum_query(dst_node<T,O>* a, int l,int r)
    {
        auto b2=prefix_query(a,r), b1=prefix_query(a,l);
        return b2-b1;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* find_closest(dst_node<T,O>* tree, T v)
    {
        if (!tree)
            return nullptr;
        if (v == tree->v)
            return tree;
        if (v > tree->v)
        {
            if (!tree->right)
                return tree;
            return find_closest(tree->right, v);
        }
        else
        {
            if (!tree->left)
                return tree;
            return find_closest(tree->left, v);
        }

    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* next(dst_node<T,O> *tree, T v)
    {
        if (!tree)
            return nullptr;
        auto dist = tree;
        if (dist->right)
        {
            dist = dist->right;
            for (; dist->left; dist = dist->left);
        }
        else
        {
            while (dist->parent)
            {
                if (dist->v > v)
                    return dist;
                dist = dist->parent;
            }
        }
        return dist;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* next(dst_node<T,O> *tree)
    {
        if (!tree)
            return nullptr;
        auto dist = tree;
        auto v=tree->v;
        if (dist->right)
        {
            dist = dist->right;
            for (; dist->left; dist = dist->left);
        }
        else
        {
            while (dist)
            {
                if (dist->v > v)
                    return dist;
                dist = dist->parent;
            }
        }
        return dist;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* prev(dst_node<T,O> *tree, T v)
    {
        if (!tree)
            return nullptr;
        auto dist = tree;
        if (dist->left)
        {
            dist = dist->left;
            for (; dist->right; dist = dist->right);
        }
        else
        {
            while (dist->parent)
            {
                if (dist->v < v)
                    return dist;
                dist = dist->parent;
            }
        }
        return dist;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* rotate_right(dst_node<T,O>* tree)
    {
        using K=dst_node<T,O>;
        auto x = tree,y=tree->left,P=tree->parent;
        if (!y)
            return tree;
        auto A = y->left, B = y->right, C = x->right;
        y->right = x;
        y->left = A;
        y->parent = x->parent;
        x->right = C;
        x->left = B;
        x->size=size(x->left)+1+size(C);
        y->size=size(y->left)+1+x->size;
        x->sum=K::F(B?B->sum:O::neutral,K::F(x->v,C?C->sum:O::neutral));
        y->sum=K::F(A?A->sum:O::neutral,K::F(y->v,x->sum));
        if (A)
            A->parent = y;
        if (P)
        {
            if (P->left == tree)
                P->left = y;
            else P->right = y;
        }
        x->parent = y;
        if (B)
            B->parent = x;
        if (C)
            C->parent = x;
        x->height = 1 + std::max(height(B), height(C));
        y->height = 1 + std::max(height(A), x->height);
        return y;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* rotate_left(dst_node<T,O>* tree)
    {
        using K=dst_node<T,O>;
        auto x = tree, y = tree->right,P=tree->parent;
        if (!y)
            return tree;
        auto A = y->right, B = y->left, C = x->left;
        y->left = x;
        y->right = A;
        y->parent = x->parent;
        x->left = C;
        x->right = B;
        x->size=size(x->left)+(B?B->size:0)+1;
        y->size=size(y->left)+1+size(A);
        x->sum=K::F(C?C->sum:O::neutral,K::F(x->v,B?B->sum:O::neutral));
        y->sum=K::F(K::F(x->sum,y->v),A?A->sum:O::neutral);

        if (P)
        {
            if (P->left == tree)
                P->left = y;
            else P->right = y;
        }
        if (A)
            A->parent = y;
        x->parent = y;
        if (B)
            B->parent = x;
        if (C)
            C->parent = x;
        x->height = 1 + std::max(height(B), height(C));
        y->height = 1 + std::max(height(A), x->height);
        return y;
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>*  rebalance_right(dst_node<T,O>* tree)
    {
        if (tree->left)
        {
            if (height(tree->left->right) - height(tree->left->left) > 0)
                tree->left=rotate_left(tree->left);

        }

        return tree=rotate_right(tree);
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* rebalance_left(dst_node<T,O>* tree)
    {
        if (tree->right)
        {
            if (height(tree->right->left) - height(tree->right->right) > 0)
                tree->right=rotate_right(tree->right);
        }
        return tree=rotate_left(tree);
    }

    template<typename T, typename O=plus_t<T>>
    void rebalance(dst_node<T,O>* tree)
    {
        if (!tree)
            return;
        tree->height = std::max(height(tree->left), height(tree->right)) + 1;
        auto D = height(tree->left) - height(tree->right);
        if (D > 1)
            tree=rebalance_right(tree);
        else if (D < -1)
            tree=rebalance_left(tree);

        tree->height = std::max(height(tree->left), height(tree->right)) + 1;
        if(tree->parent)
            rebalance(tree->parent);
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* insert(dst_node<T,O>*& tree, T v,int o)
    {
        using K=dst_node<T,O>;
        if (!tree)
        {
            tree = new dst_node<T,O>();
            tree->v = v;
            tree->parent = nullptr;
            tree->left = nullptr;
            tree->right = nullptr;
            tree->height = 1;
            tree->size=1;
            tree->sum=v;
            return tree;
        }
        bool insert_left=o<tree->size;
        auto dist = find_by_order(tree,o);
        if(dist && dist->left)
        {
            for (dist = dist->left; dist->right; dist = dist->right);
            insert_left=false;
        }
        if(dist==nullptr)
        {
            dist=tree;
            while(dist->right)
                dist=dist->right;
        }
        auto p = new dst_node<T,O>(),q=p;
        p->v = v;
        p->height = 1;
        p->size=1;
        p->right = nullptr;
        p->left = nullptr;
        p->parent = dist;
        p->sum=O::neutral;
        if (insert_left)
        {
            dist->left = p;
            dist->sum=K::F(p->sum,K::F(dist->v,dist->right?dist->right->sum:O::neutral));
            dist->size=size(dist->left)+1+size(dist->right);
        }
        else {
            dist->right = p;
            dist->sum=K::F(K::F(dist->left?dist->left->sum:O::neutral,dist->v),p->sum);
            dist->size=size(dist->left)+1+p->size;
        }
        dist->height = std::max(height(dist->right), height(dist->left)) + 1;
        while(p)
        {
            p->size=size(p->left)+1+size(p->right);
            p->sum=K::F(p->left?p->left->sum:O::neutral,K::F(p->v,p->right?p->right->sum:O::neutral));
            p=p->parent;
        }
        rebalance(dist);
        return q;
    }

    template<typename T, typename O=plus_t<T>>
    void remove(dst_node<T,O>*& tree)
    {
        if (!tree)
            return;
        if (!tree->right)
        {
            if(tree->left)
                tree->left->parent = tree->parent;
            if (tree->parent)
            {
                if (tree->parent->left == tree)
                    tree->parent->left = tree->left;
                else tree->parent->right = tree->left;
            }
            auto s= tree;
            tree = tree->left;
            delete s;
            rebalance(tree);
        }
        else if (!tree->right->left)
        {
            auto s = tree->right;
            s->left = tree->left;
            s->parent = tree->parent;
            if (s->parent)
            {
                if (tree->parent->left == tree)
                    tree->parent->left = s;
                else tree->parent->right = s;
            }
            if (tree->left)
                tree->left->parent = s;
            delete tree;
            tree = s;
            rebalance(s);
        }
        else
        {
            auto s = tree->right;
            while (s->left)
                s = s->left;
            auto w = s->parent;
            w->left = s->right;
            if (s->right)
                s->right->parent = w;
            s->left = tree->left;
            s->right = tree->right;
            s->parent = tree->parent;
            if (s->parent)
            {
                if (tree->parent->left == tree)
                    tree->parent->left = s;
                else tree->parent->right = s;
            }
            if (tree->left)
                tree->left->parent = s;
            tree->right->parent = s;
            delete tree;
            tree = s;
            rebalance(w);
        }
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* extract(dst_node<T,O>*& tree)
    {
        if (!tree)
            return nullptr;
        if (!tree->right)
        {
            if (tree->left)
                tree->left->parent = tree->parent;
            if (tree->parent)
            {
                if (tree->parent->left == tree)
                    tree->parent->left = tree->left;
                else tree->parent->right = tree->left;
            }
            auto s = tree;
            if (s)
            {
                s->left = nullptr;
                s->right = nullptr;
                s->parent = nullptr;
            }
            tree = tree->left;
            rebalance(tree);
            return s;
        }
        else
        {
            auto s = tree->right;
            while (s->left)
                s = s->left;
            auto w = s->parent;
            w->left = s->right;
            if (s->right)
                s->right->parent = w;
            s->left = tree->left;
            s->right = tree->right;
            s->parent = tree->parent;
            if (s->parent)
            {
                if (tree->parent->left == tree)
                    tree->parent->left = s;
                else tree->parent->right = s;
            }
            if (tree->left)
                tree->left->parent = s;
            tree->right->parent = s;
            auto p = tree;
            tree = s;
            rebalance(p);
            if (p)
            {
                p->left = nullptr;
                p->right = nullptr;
                p->parent = nullptr;
            }
            return p;

        }
    }

    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>*  merge(dst_node<T,O>* t1,dst_node<T,O>* t2)
    {
        auto diff_H = height(t2) - height(t1);
        if (!t1)
            return t2;
        else if (!t2)
            return t1;
        if (diff_H > 1)
            return merge(t1, t2->left);
        else if (diff_H < -1)
            return merge(t1->right, t2);
        else
        {
            dst_node<T,O>* w;
            if (diff_H >= 0)
            {
                w = t2;
                while (w->left)
                    w = w->left;
                w=extract(w);
                if (w == t2)
                    t2 = nullptr;
            }
            else
            {
                w = t1;
                while (w->right)
                    w = w->right;
                w=extract(w);
                if (w == t1)
                    t1 = nullptr;
            }
            w->left = t1;
            w->right = t2;
            if (t1)
            {
                if(t1->parent)
                    w->parent = t1->parent;
                t1->parent = w;
            }
            if(t2)
            {
                if(t2->parent)
                    w->parent = t2->parent;
                t2->parent = w;
            }
            while (w->parent)
            {
                w->height = std::max(height(w->left), height(w->right)) + 1;
                w = w->parent;
            }
            w->height = std::max(height(w->left), height(w->right)) + 1;
            return w;
        }
    }


    template<typename T, typename O=plus_t<T>>
    dst_node<T,O>* merge_with_root(dst_node<T,O>* t1, dst_node<T,O>* t2,dst_node<T,O> *root)
    {
        auto diff_H = height(t2) - height(t1);
        if (diff_H > 1)
            return merge_with_root(t1, t2->left,root);
        else if (diff_H < -1)
            return merge_with_root(t1->right, t2,root);
        else
        {
            root->left = t1;
            root->right = t2;
            if(t1)
                t1->parent = root;
            if(t2)
                t2->parent = root;
            rebalance(root);
            return root;
        }
    }

    template<typename T, typename O=plus_t<T>>
    std::pair<dst_node<T,O>*, dst_node<T,O>*> split(dst_node<T,O>* tree,T v)
    {
        auto w = tree;
        if (!w)
            return {nullptr,nullptr};

        auto l = tree->left, r = tree->right;
        tree->height = 1;
        dst_node<T,O> *S1, *S2;
        if (v > tree->v)
        {
            auto P = split(r, v);
            S1 = merge_with_root(l, P.first,tree);
            S2 = P.second;
        }
        else
        {
            auto P = split(l, v);
            S2 = merge_with_root(P.second, r, tree);
            S1 = P.first;
        }
        return { S1,S2 };
    }

    template<typename T, typename O=plus_t<T>>
    void destroy(dst_node<T,O>*A)
    {
        if(!A)
            return;
        destroy(A->left);
        destroy(A->right);
        delete A;
    }
}

template<typename T, typename O=plus_t<T>>
class dynamic_segment_tree
{
    dst_node<T,O>* root,*start;
public:
    dynamic_segment_tree():root(nullptr){}
    ~dynamic_segment_tree()
    {
        dst_utils::destroy(root);
        root=nullptr;
    }
    bool find(T a) const
    {
        return dst_utils::find(root, a);
    }

    dst_node<T,O>* find_closest(T a) const
    {
        return dst_utils::find_closest(root, a);
    }


    void insert(T a)
    {
       insert(a,root?root->size:0);
    }

    void insert(T a,std::size_t pos)
    {
        auto p=dst_utils::insert(root, a,pos);
        if(root->size==1)
            start=root;
        while (root->parent)
            root = root->parent;
        if(start->v>a)
            start=p;
    }

    void remove(dst_node<T,O>*& tree)
    {
        if (tree == root)
            dst_utils::remove(root);
        else dst_utils::remove(tree);
    }

    void remove(T v)
    {
        if (!root)
            return;
        auto p = find_closest(v);
        if (p->v != v)
            return;
        remove(p);
    }

    int height() const
    {
        return dst_utils::height(root);
    }

    dst_node<T,O>* get_root()
    {
        return root;
    }
    void rotate_left(dst_node<T,O>* tree)
    {
        dst_utils::rotate_left(tree);
        if (root->parent)
            root = root->parent;
    }

    void rotate_right(dst_node<T,O>* tree)
    {
        dst_utils::rotate_right(tree);
        if (root->parent)
            root = root->parent;
    }

    dst_node<T,O>* begin() const
    {
        return start;
    }

    void merge(dynamic_segment_tree&& other)
    {
        root = dst_utils::merge(root, other.root);
        other.root = nullptr;
    }
    static std::pair<dynamic_segment_tree, dynamic_segment_tree> split(dynamic_segment_tree&& t,T v)
    {
        dynamic_segment_tree t1, t2;
        auto P = dst_utils::split(t.root, v);
        t1.root = P.first;
        t2.root = P.second;
        t.root = nullptr;
        return { t1,t2 };
    }
    T query(int l,int r) const
    {
        return dst_utils::query(root,l,r);
    }

    T sum_query(int l,int r) const
    {
        return dst_utils::sum_query(root,l,r);
    }

    void update(int p,const T&a)
    {
        using K = dst_node<T, O>;
        auto u = dst_utils::find_by_order(root, p);
        u->v = a;
        while (u) {
            u->sum = K::F(u->left ? u->left->sum : O::neutral, K::F(u->v, u->right ? u->right->sum : O::neutral));
            u = u->parent;
        }
    }
};


#endif //ALGORITHMICLIBRARY_DYNAMIC_SEGMENT_TREE_H
