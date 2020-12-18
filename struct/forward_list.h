#pragma once
#pragma once

template<typename T>
class list
{
	struct node
	{
		T v;
		node* next, * prev;
	} *head, * tail;
	int L;
public:
	list() :head(nullptr), tail(nullptr), L(0) {}
	~list() {}
	void push_back(T s)
	{
		if (!head)
		{
			head = new T(s);
			tail = head;
			head->prev = nullptr;
			head->next = nullptr;
			return;
		}
		auto P = new T(s);
		tail->next = P;
		P->prev = tail;
		P->next = nullptr;
		tail = P;
		L++;
	}

	void push_front(T s)
	{
		if (!head)
		{
			head = new T(s);
			tail = head;
			head->prev = nullptr;
			head->next = nullptr;
			return;
		}
		auto P = new T(s);
		head->prev = P;
		P->next = head;
		P->prev = nullptr;
		head = P;
		L++;
	}

	void remove(node* N)
	{
		if (N != head)
			N->prev->next = N->next;
		if (N != tail)
			N->next->prev = N->prev;
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

};

