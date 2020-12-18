#pragma once
#include <iostream>
#include <algorithm>


template<typename T>
struct bst_node
{
	T v,height;
	bst_node * left, * right, *parent;
};
namespace bst_utils
{

	template<typename T>
	int height(bst_node<T>* tree)
	{
		if (!tree)
			return 0;
		return tree->height;
	}

	template<typename T>
	bool is_avl(bst_node<T>* tree);
	template<typename T>
	bool find(bst_node<T>* a, T v)
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
	bst_node<T>* find_closest(bst_node<T>* tree, T v)
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

	template<typename T>
	bst_node<T>* next(bst_node<T> *tree, T v)
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
	bst_node<T>* rotate_right(bst_node<T>* tree)
	{
		auto x = tree,y=tree->left,P=tree->parent;
		if (!y)
			return tree;
		auto A = y->left, B = y->right, C = x->right;
		y->right = x;
		y->left = A;
		y->parent = x->parent;
		x->right = C;
		x->left = B;
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
	bst_node<T>* rotate_left(bst_node<T>* tree)
	{
		auto x = tree, y = tree->right,P=tree->parent;
		if (!y)
			return tree;
		auto A = y->right, B = y->left, C = x->left;
		y->left = x;
		y->right = A;
		y->parent = x->parent;
		x->left = C;
		x->right = B;
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
	bst_node<T>*  rebalance_right(bst_node<T>* tree)
	{
		if (tree->left)
		{
			if (height(tree->left->right) - height(tree->left->left) > 0)
				tree->left=rotate_left(tree->left);
			
		}

		return tree=rotate_right(tree);
	}

	template<typename T>
	bst_node<T>* rebalance_left(bst_node<T>* tree)
	{
		if (tree->right)
		{
			if (height(tree->right->left) - height(tree->right->right) > 0)
				tree->right=rotate_right(tree->right);
		}
		return tree=rotate_left(tree);
	}

	template<typename T>
	void rebalance(bst_node<T>* tree)
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
	void insert(bst_node<T>*& tree, T v)
	{
		if (!tree)
		{
			tree = new bst_node<T>();
			tree->v = v;
			tree->parent = nullptr;
			tree->left = nullptr;
			tree->right = nullptr;
			tree->height = 1;
			return;
		}
		auto dist = find_closest(tree, v);
		auto p = new bst_node<T>();
		p->v = v;
		p->height = 1;
		p->right = nullptr;
		p->left = nullptr;
		p->parent = dist;
		if (v > dist->v)
			dist->right = p;
		else
			dist->left = p;
		dist->height = std::max(height(dist->right), height(dist->left)) + 1;
		rebalance(dist);
	}

	template<typename T>
	void remove(bst_node<T>*& tree)
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

	template<typename T>
	bst_node<T>* extract(bst_node<T>*& tree)
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

	template<typename T>
	void print(bst_node<T>* tree)
	{
		if (!tree)
			return;
		print(tree->left);
		std::cout << tree->v << '\t';
		print(tree->right);
	}

	template<typename T>
	bool is_greaterequal_thanall(T v,bst_node<T> *tree)
	{
		if (!tree)
			return true;
		return (v >= tree->v) && is_greaterequal_thanall(v, tree->left) && is_greaterequal_thanall(v, tree->right);
	}

	template<typename T>
	bool is_lessequal_thanall(T v, bst_node<T>* tree)
	{
		if (!tree)
			return true;
		return (v <= tree->v) && is_lessequal_thanall(v, tree->left) && is_lessequal_thanall(v, tree->right);
	}

	template<typename T>
	bool is_bst(bst_node<T>* tree)
	{
		if (!tree)
			return true;
		return is_greaterequal_thanall<T>(tree->v, tree->left) && is_lessequal_thanall<T>(tree->v, tree->right)
			&& is_bst(tree->left) && is_bst(tree->right);
	}

	template<typename T>
	bool is_avl(bst_node<T>* tree)
	{
		if (!tree)
			return true;
		if (std::abs(height(tree->left) - height(tree->right)) > 1)
		{
			int a = 0;
		}
		return (std::abs(height(tree->left) - height(tree->right)) <= 1) && is_avl(tree->left) && is_avl(tree->right);
	}

	template<typename T>
	bst_node<T>*  merge(bst_node<T>* t1,bst_node<T>* t2)
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
			bst_node<T>* w;
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


	template<typename T>
	bst_node<T>* merge_with_root(bst_node<T>* t1, bst_node<T>* t2,bst_node<T> *root)
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

	template<typename T>
	std::pair<bst_node<T>*, bst_node<T>*> split(bst_node<T>* tree,T v)
	{
		auto w = tree;
		if (!w)
			return {nullptr,nullptr};

		auto l = tree->left, r = tree->right;
		tree->height = 1;
		bst_node<T> *S1, *S2;
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
}

template<typename T>
class binary_search_tree
{
	bst_node<T>* root;
public:
	binary_search_tree():root(nullptr){}
	bool find(T a) const
	{
		return bst_utils::find(root, a);
	}

	bst_node<T>* find_closest(T a) const
	{
		return bst_utils::find_closest(root, a);
	}

	void insert(T a)
	{
		bst_utils::insert(root, a);
		while (root->parent)
			root = root->parent;
	}

	void remove(bst_node<T>*& tree)
	{
		if (tree == root)
			bst_utils::remove(root);
		else bst_utils::remove(tree);
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

	void print() const
	{
		bst_utils::print(root);
	}

	int height() const
	{
		return bst_utils::height(root);
	}

	bst_node<T>* get_root()
	{
		return root;
	}
	void rotate_left(bst_node<T>* tree)
	{
		bst_utils::rotate_left(tree);
		if (root->parent)
			root = root->parent;
	}

	void rotate_right(bst_node<T>* tree)
	{
		bst_utils::rotate_right(tree);
		if (root->parent)
			root = root->parent;
	}

	void merge(binary_search_tree&& other)
	{
		root = bst_utils::merge(root, other.root);
		other.root = nullptr;
	}
	static std::pair<binary_search_tree, binary_search_tree> split(binary_search_tree&& t,T v)
	{
		binary_search_tree t1, t2;
		auto P = bst_utils::split(t.root, v);
		t1.root = P.first;
		t2.root = P.second;
		t.root = nullptr;
		return { t1,t2 };
	}
};