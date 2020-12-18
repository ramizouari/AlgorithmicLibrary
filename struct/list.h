#pragma once
#include <cassert>

template<typename T>
class list
{
public :
	struct node
	{
		T v;
		node* next, * prev;
	};
private:
	node *head, * tail;
	int L;
public:
	list() :head(nullptr), tail(nullptr),L(0) {}
	list(const list& O)
	{
		auto p = O.head;
		while (p != nullptr)
		{
			push_back(p->v);
			p = p->next;
		}
	}

	list& operator=(const list& O)
	{
		auto q = head;
		auto p = O.head;
		while ((p != nullptr)&&(q!=nullptr))
		{
			q->v = p->v;
			p = p->next;
			q = q->next;
		}

		while (p != nullptr)
		{
			push_back(p->v);
			p = p->next;
		}

		if (q != nullptr)
		{
			while (q != tail)
				pop_back();
			pop_back();
		}
		return *this;
	}

	list(list&& O) noexcept :head(std::move(O.head)),tail(std::move(O.tail)),L(O.L)
	{
		O.head = nullptr;
		O.tail = nullptr;
		O.L = 0;
	}
	~list() 
	{
		auto p = head, q = p;
		while (p != nullptr)
		{
			p = p->next;
			delete q;
			q = p;
		}
	}
	void push_back(T s) 
	{
		L++;
		if (!head)
		{
			head = new node;
			head->v = s;
			tail = head;
			head->prev = nullptr;
			head->next = nullptr;
			return;
		}
		auto P = new node;
		P->v = s;
		tail->next = P;
		P->prev = tail;
		P->next = nullptr;
		tail = P;
	}

	void push_front(T s)
	{
		L++;
		if (!head)
		{
			head = new node;
			head->v = s;
			tail = head;
			head->prev = nullptr;
			head->next = nullptr;
			return;
		}
		auto P = new node;
		P->v = s;
		head->prev = P;
		P->next = head;
		P->prev = nullptr;
		head = P;
	}

	void remove(node* N)
	{
		if (N != head)
			N->prev->next = N->next;
		else head = head->next;
		if (N != tail)
			N->next->prev = N->prev;
		else tail = tail->prev;
		delete N;
		L--;
	}

	void pop()
	{
		auto P = head;
		head = head->next;
		if (head == nullptr)
			tail = nullptr;
		else
			head->prev = nullptr;
		delete P;
		L--;
	}

	void pop_back()
	{
		auto P = tail;
		tail = tail->prev;
		if (tail == nullptr)
			head = nullptr;
		else
			tail->next = nullptr;
		delete P;
		L--;
	}

	node* find_node(T w) const
	{
		auto P = head;
		for (P; P != nullptr; P = P->next)
			if (P->v == w)
				return P;
		return nullptr;
	}
	node* get_head() const
	{
		return head;
	}
	int get_length() const
	{
		return L;
	}

	T& front()
	{
		return head->v;
	}

	T& back()
	{
		return tail->v;
	}
};

