//
// Created by ramizouari on 06/10/2021.
//

#ifndef ALGORITHMICLIBRARY_ORDER_STATISTIC_TREE_H
#define ALGORITHMICLIBRARY_ORDER_STATISTIC_TREE_H


//
// Created by ramizouari on 25/09/2021.
//
/*
 * An Ordered Segment Tree is a Data Structure supporting:
 * 1. Insertion and Deletion in O(ln n)
 * 2. Range Queries on the Sorted Data in O(ln n)
 * 3. Order of an element in O(ln n)
 * 4. Number of elements in a range in O(ln n)
 * 5. O(n) memory
 *
 * */

#include "binary_operator.h"


template<typename T>
struct ordered_stat_node
{
    inline static O F=O();
    T v;
    int height,size;
    ordered_stat_node * left, * right, *parent;
};

namespace ordered_stat_node_utils
{

    template<typename T>
    inline int height(ordered_stat_node<T,O>* tree)
    {
        return tree?tree->height:0;
    }

    template<typename T>
    inline int size(ordered_stat_node<T,O>* tree)
    {
        return tree?tree->size:0;
    }

    template<typename T>
    ordered_stat_node<T,O>* clone(ordered_stat_node<T,O> * u)
    {
        if(!u)
            return nullptr;
        auto s= new ordered_stat_node<T,O>;
        s->v=u->v;
        s->height=u->height;
        s->size=u->size;
        s->parent=nullptr;
        auto p=clone(u->left);
        auto q=clone(u->right);
        if(p)
            p->parent=s;
        if(q)
            q->parent=s;
        s->left=p;
        s->right=q;
        return s;
    }

    template<typename T>
    bool find(ordered_stat_node<T,O>* a, T v)
    {
        if (!a)
            return false;
        if (v == a->v)
            return true;
        else if (v > a->v)
            return find(a->right, v);
        else return find(a->left, v);
    }

    template<typename T>
    ordered_stat_node<T,O>* find_by_order(ordered_stat_node<T,O>* a, int order)
    {
        if(!a)
            return nullptr;
        if(size(a->left)==order)
            return a;
        else if(size(a->left)>order)
            return find_by_order(a->left,order);
        else return find_by_order(a->right,order-size(a->left)-1);
    }

    template<typename T>
    int order(ordered_stat_node<T,O>* a, T v,int offset=0)
    {
        if (!a)
            return -1;
        if (v == a->v)
            return offset+(a?size(a->left):0);
        else if (v > a->v)
            return order(a->right, v,offset+1+(a?size(a->left):0));
        else return order(a->left, v,offset);
    }

    template<typename T>
    ordered_stat_node<T,O>* upper_bound(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return nullptr;
        if(a->v > u)
        {
            auto p=upper_bound(a->left,u);
            if(!p)
                return a;
            return p;
        }
        else return upper_bound(a->right,u);
    }

    template<typename T>
    ordered_stat_node<T,O>* lower_bound(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return nullptr;
        if(a->v > u)
        {
            auto p=lower_bound(a->left,u);
            if(!p)
                return a;
            return p;
        }
        else if(a->v==u)
            return a;
        else return lower_bound(a->right,u);
    }

    template<typename T>
    int lower_bound_order(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return 0;
        if(a->v > u)
            return lower_bound_order(a->left,u);
        else if(a->v==u)
            return size(a->left);
        else return size(a->left)+1+lower_bound_order(a->right,u);
    }

    template<typename T>
    std::pair<int,ordered_stat_node<T,O>*> lower_bound_pair(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return {0,nullptr};
        if(a->v > u) {
            auto [o,p]= lower_bound_pair(a->left, u);
            return {o,p?p:a};
        }
        else if(a->v==u)
            return {size(a->left),a};
        else {
            auto [o,p]=lower_bound_pair(a->right, u);
            return {size(a->left) + 1 + o,p};
        }
    }

    template<typename T>
    std::pair<int,ordered_stat_node<T,O>*> upper_bound_pair(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return {0,nullptr};
        if(a->v > u)
        {
            auto [o,p]= upper_bound_pair(a->left, u);
            return {o,p?p:a};
        }
        else {
            auto [o,p]=upper_bound_pair(a->right, u);
            return {size(a->left) + 1 + o,p};
        }
    }

    template<typename T>
    int upper_bound_order(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return 0;
        if(a->v > u)
            return upper_bound_order(a->left,u);
        else return size(a->left)+1+upper_bound_order(a->right,u);
    }

    template<typename T>
    ordered_stat_node<T,O>* reverse_lower_bound(ordered_stat_node<T,O>*a,T u)
    {
        if(!a)
            return nullptr;
        if(a->v < u)
        {
            auto p=reverse_lower_bound(a->right,u);
            if(!p)
                return a;
            return p;
        }
        else if(a->v==u)
            return a;
        else return reverse_lower_bound(a->left,u);
    }

    template<typename T>
    ordered_stat_node<T,O>* find_closest(ordered_stat_node<T,O>* tree, T v)
    {
        if (!tree)
            return nullptr;
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

    template<typename T>
    ordered_stat_node<T,O>* next(ordered_stat_node<T,O> *tree, T v)
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

    template<typename T>
    ordered_stat_node<T,O>* next(ordered_stat_node<T,O> *tree)
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

    template<typename T>
    ordered_stat_node<T,O>* prev(ordered_stat_node<T,O> *tree, T v)
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

    template<typename T>
    ordered_stat_node<T,O>* rotate_right(ordered_stat_node<T,O>* tree)
    {
        using K=ordered_stat_node<T,O>;
        auto x = tree,y=tree->left,P=tree->parent;
        if (!y)
            return tree;
        auto A = y->left, B = y->right, C = x->right;
        y->right = x;
        y->left = A;
        y->parent = x->parent;
        x->right = C;
        x->left = B;
        x->size=size(x->left)+1+(C?C->size:0);
        y->size=size(y->left)+1+x->size;
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

    template<typename T>
    ordered_stat_node<T,O>* rotate_left(ordered_stat_node<T,O>* tree)
    {
        using K=ordered_stat_node<T,O>;
        auto x = tree, y = tree->right,P=tree->parent;
        if (!y)
            return tree;
        auto A = y->right, B = y->left, C = x->left;
        y->left = x;
        y->right = A;
        y->parent = x->parent;
        x->left = C;
        x->right = B;
        x->size=size(x->left)+size(B)+1;
        y->size=size(y->left)+1+size(A);

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

    template<typename T>
    ordered_stat_node<T,O>*  rebalance_right(ordered_stat_node<T,O>* tree)
    {
        if (tree->left)
        {
            if (height(tree->left->right) - height(tree->left->left) > 0)
                tree->left=rotate_left(tree->left);

        }

        return tree=rotate_right(tree);
    }

    template<typename T>
    ordered_stat_node<T,O>* rebalance_left(ordered_stat_node<T,O>* tree)
    {
        if (tree->right)
        {
            if (height(tree->right->left) - height(tree->right->right) > 0)
                tree->right=rotate_right(tree->right);
        }
        return tree=rotate_left(tree);
    }

    template<typename T>
    void rebalance(ordered_stat_node<T,O>* tree)
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

    template<typename T>
    ordered_stat_node<T,O>* insert(ordered_stat_node<T,O>*& tree, T v)
    {
        using K=ordered_stat_node<T,O>;
        if (!tree)
        {
            tree = new ordered_stat_node<T,O>();
            tree->v = v;
            tree->parent = nullptr;
            tree->left = nullptr;
            tree->right = nullptr;
            tree->height = 1;
            tree->size=1;
            return tree;
        }
        auto dist = find_closest(tree,v);
        if(dist==nullptr)
        {
            dist=tree;
            while(dist->right)
                dist=dist->right;
        }
        auto p = new ordered_stat_node<T,O>(),q=p;
        p->v = v;
        p->height = 1;
        p->size=1;
        p->right = nullptr;
        p->left = nullptr;
        p->parent = dist;
        if (v > dist->v)
        {
            dist->right = p;
            dist->size=(size(dist->left)+1+p->size);
        }
        else {
            dist->left = p;
            dist->size=size(dist->left)+1+size(dist->right);
        }
        dist->height = std::max(height(dist->right), height(dist->left)) + 1;
        while(p)
        {
            p->size=size(p->left)+1+size(p->right);
            p=p->parent;
        }
        rebalance(dist);
        return q;
    }

    template<typename T>
    void remove(ordered_stat_node<T,O>*& tree)
    {
        using K=ordered_stat_node<T,O>;
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
            auto s= tree,p=tree->parent;
            tree = tree->left;
            delete s;
            while(p)
            {
                p->size=size(p->left)+1+size(p->right);
                p=p->parent;
                p->height=1+std::max(height(p->left),height(p->right));
            }
            rebalance(tree);
        }
        else if (!tree->right->left)
        {
            auto s = tree->right,p=tree->parent;
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
            while(p)
            {
                p->size=size(p->left)+1+size(p->right);
                p=p->parent;
                p->height=std::max(height(p->left),height(p->right))+1;
            }
            rebalance(s);
        }
        else
        {
            auto s = tree->right;
            while (s->left)
                s = s->left;
            auto w = s->parent,p=w;
            w->left = s->right;
            if (s->right)
                s->right->parent = w;
            s->left = tree->left;
            s->right = tree->right;
            s->parent = tree->parent;
            s->size=size(s->left)+size(s->right)+1;
            s->height=std::max(heigh(s->left),heigh(s->right))+1;
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
            while(p)
            {
                p->size=size(p->left)+1+size(p->right);
                p=p->parent;
                p->height=std::max(heigh(p->left),height(p->right))+1;
            }
            rebalance(w);
        }
    }

    template<typename T>
    void destroy(ordered_stat_node<T,O>*A)
    {
        if(!A)
            return;
        destroy(A->left);
        destroy(A->right);
        delete A;
    }
}

template<typename T>
class ordered_statistic_tree
{
    ordered_stat_node<T,O>* root,*start;
public:
    ordered_statistic_tree():root(nullptr){}
    ordered_statistic_tree(const ordered_statistic_tree &o):root(ordered_stat_node_utils::clone(o.root))
    {
        start=root;
        if(start) while(start->left)
                start=start->left;
    }
    ordered_statistic_tree(ordered_statistic_tree &&o):root(std::move(o.root))
    {
        o.root=nullptr;
    }
    ~ordered_statistic_tree()
    {
        ordered_stat_node_utils::destroy(root);
        root=nullptr;
    }
    bool find(T a) const
    {
        return ordered_stat_node_utils::find(root, a);
    }

    ordered_stat_node<T,O>* find_closest(T a) const
    {
        return ordered_stat_node_utils::find_closest(root, a);
    }

    void insert(T a)
    {
        auto p=ordered_stat_node_utils::insert(root, a);
        if(root->size==1)
            start=root;
        while (root->parent)
            root = root->parent;
        if(start->v>a)
            start=p;
    }

    void remove(ordered_stat_node<T,O>*& tree)
    {
        if (tree == root)
            ordered_stat_node_utils::remove(root);
        else ordered_stat_node_utils::remove(tree);
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

    int order(T u) const
    {
        return ordered_stat_node_utils::order(root,u);
    }

    int rank(T u) const
    {
        return order(u);
    }

    T select(int u) const
    {
        return ordered_stat_node_utils::find(root,u)->v;
    }

    int count_range(T u,T v) const
    {
        auto p=ordered_stat_node_utils::lower_bound(root,u),q=ordered_stat_node_utils::upper_bound(root,v);
        if(u>v)
            return 0;
        if(!p || p->v>v)
            return 0;
        return (q?order(q->v):root->size)-order(p->v);
    }

    std::pair<int,int> bound(T u,T v) const
    {
        return {ordered_stat_node_utils::lower_bound_order(root,u),ordered_stat_node_utils::upper_bound_order(root,v)};
    }

    std::tuple<int,int,ordered_stat_node<T,O>*,ordered_stat_node<T,O>*> bound_info(T u,T v) const
    {
        auto [o1,p1]=ordered_stat_node_utils::lower_bound_pair(root,u);
        auto [o2,p2]=ordered_stat_node_utils::upper_bound_pair(root,v);
        return {o1,o2,p1,p2};
    }

    int height() const
    {
        return ordered_stat_node_utils::height(root);
    }

    ordered_stat_node<T,O>* get_root()
    {
        return root;
    }
    void rotate_left(ordered_stat_node<T,O>* tree)
    {
        ordered_stat_node_utils::rotate_left(tree);
        if (root->parent)
            root = root->parent;
    }

    void rotate_right(ordered_stat_node<T,O>* tree)
    {
        ordered_stat_node_utils::rotate_right(tree);
        if (root->parent)
            root = root->parent;
    }

    ordered_stat_node<T,O>* begin() const
    {
        return start;
    }

}

#endif //ALGORITHMICLIBRARY_ORDER_STATISTIC_TREE_H
