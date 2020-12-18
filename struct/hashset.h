#pragma once
#include "dynamic_array.h"
#include "list.h"
#include "hash.h"
#include <random>
#include <optional>



template<typename T,typename hash=linear_universal_hash_function<T>>
class dynamic_hashtable
{
	dynamic_array<list<T>> H;
	hash f;
public:
	dynamic_hashtable(dynamic_hashtable && S) :H(std::move(S.H)),f(std::move(S.f))
	{
	}

	dynamic_hashtable(int m):H(m),f(m)
	{
	}

	dynamic_hashtable&  operator= (dynamic_hashtable&&) = default;

	void insert(T w)
	{
		auto R = f(w);
		auto P = H[R].find_node(w);
		if (P)
			return;
		H[R].push_front(w);
	}

	void remove(T w)
	{
		auto R = f(w);
		auto P = H[R].find_node(w);
		if (!P)
			return;
		H[R].remove(P);
	}

	bool find(T w) const
	{
		auto R = f(w);
		auto P = H[R].find_node(w);
		return P != nullptr;
	}
	void rehash(int s)
	{
		int m = H.size();
		dynamic_array<list<T>> S(s);
		hash g(s);
		for (int i = 0; i < m; i++)
			for (auto p = H[i].get_head(); p != nullptr; p = p->next)
				S[g(p->v)].push_front(p->v);
		this->H = std::move(S);
		this->f = std::move(g);
	}
	
	void rehash()
	{
		int m = H.size();
		dynamic_array<list<T>> S(m);
		hash g(m);
		for (int i = 0; i < m; i++)
			for (auto p = H[i].get_head(); p != nullptr; p = p->next)
				S[g(p->v)].push_front(p->v);
		this->H = std::move(S);
		this->f = std::move(g);
	}
};