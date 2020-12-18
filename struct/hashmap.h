#pragma once
#include "dynamic_array.h"
#include "list.h"
#include "hash.h"
#include <random>
#include <optional>
#include <utility>
#include <string>



template<typename T1,typename T2, typename hash = linear_universal_hash_function<T1>>
class dynamic_hashmap
{
	using couple = std::pair<T1, T2>;
	dynamic_array <list<couple>> H;
	hash f;
public:
	dynamic_hashmap(dynamic_hashmap&& S) :H(std::move(S.H)), f(std::move(S.f))
	{
	}

	dynamic_hashmap(int m) :H(m), f(m)
	{
	}

	dynamic_hashmap& operator= (dynamic_hashmap&&) = default;

	void set(T1 k,T2 x)
	{
		auto R = f(k);
		auto P = get_key_node(k);
		if (P)
			P->v.second = x;
		else H[f(k)].push_front({k,x});
	}

	void remove(T1 k)
	{
		auto P = get_key_node(k);
		if (!P)
			return;
		H[f(k)].remove(P);
	}

	T2& operator[](T1 k)
	{
		auto P = get_key_node(k);
		int s = f(k);

		if (!P)
		{
			H[s].push_front({ k,T2() });
			return H[s].back().second;

		}
		else return P->v.second;
			
	}

	T2& get(T1 k)
	{
		auto P = get_key_node(k);
		return P->v.second;
	}

	const T2& get(T1 k) const
	{
		auto P = get_key_node(k);
		return P->v.second;
	}

	bool find(T1 k) const
	{
		auto P = get_key_node(k);
		return P != nullptr;
	}
	void rehash(int s)
	{
		int m = H.size();
		dynamic_array<list<couple>> S(s);
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
		dynamic_array<list<couple>> S(m);
		hash g(m);
		for (int i = 0; i < m; i++)
			for (auto p = H[i].get_head(); p != nullptr; p = p->next)
				S[g(p->v)].push_front(p->v);
		this->H = std::move(S);
		this->f = std::move(g);
	}
protected:
	auto get_key_node(T1 k) const
	{
		auto R = f(k);
		auto P = H[R].get_head();
		while (P != nullptr)
		{
			if (P->v.first == k)
			{
				return P;
			}
			P = P->next;
		}
		return P;
	}
};