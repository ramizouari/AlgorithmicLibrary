#pragma once
#include <random>
template<typename T>
class abstract_hash_function
{
public:
	abstract_hash_function(std::uint64_t _m) :m(_m) {}
	virtual std::uint64_t apply(T w) const = 0;
	std::uint64_t operator()(T w) const
	{
		return apply(w);
	}
protected:
	std::uint64_t m;

};

//this linear hash function is easy to code, but it is vulnerable to collisions
template<typename T>
class modulo_function : public abstract_hash_function<T>
{
public:
	modulo_function(std::uint64_t _m) :abstract_hash_function<T>(_m)
	{
	}

	std::uint64_t apply(T w) const
	{
		return w % this->m;
	}
};


//this linear hash function is easy to code, but it is vulnerable to collisions
template<typename T>
class linear_hash_function : public abstract_hash_function<T>
{
	std::uint64_t a, b;
public:
	linear_hash_function(std::uint64_t _m) :abstract_hash_function<T>(_m)
	{

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib(0, this->m - 1);
		a = distrib(gen);
		b = distrib(gen);
	}

	linear_hash_function(std::uint64_t _m, std::uint64_t seed) :abstract_hash_function<T>(_m)
	{

		std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib(1, this->m - 1);
		a = distrib(gen);
		b = distrib(gen);
	}

	std::uint64_t apply(T w) const
	{
		return ((a * w) % this->m + b) % this->m;
	}
};

//This is a linear universal hashfunction
//It is universal in the sense that given different x and x' the probability that there is a collision between these two is less 
// than 1/m
//The considered random variable is the hashfunction itself
//The assumptions made are
// - the universal set U is bounded by -M and M where M < p
// - p is prime
template<typename T,std::uint64_t p= 100003>
class linear_universal_hash_function : public abstract_hash_function<T>
{
	std::uint64_t a, b;
public:
	linear_universal_hash_function(std::uint64_t _m) :abstract_hash_function<T>(_m)
	{

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib1(1, p - 1);
		std::uniform_int_distribution<> distrib2(0, p - 1);
		a = distrib1(gen);
		b = distrib2(gen);
	}

	linear_universal_hash_function(std::uint64_t _m, std::uint64_t seed) :abstract_hash_function<T>(_m)
	{

		std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<std::uint64_t> distrib1(1, p - 1);
		std::uniform_int_distribution<std::uint64_t> distrib2(0,p - 1);
		a = distrib1(gen);
		b = distrib2(gen);
	}

	std::uint64_t apply(T w) const
	{
		return (((a * w) % p + b) % p)%this->m;
	}
};

class string_sum_hash :public abstract_hash_function<std::string>
{
public:
	string_sum_hash(int m):abstract_hash_function<std::string>(m){}
	std::uint64_t apply(std::string s)
	{
		std::uint64_t k(0);
		for (auto& c : s)
			k += c;
		return k%this->m;
	}
};

template<typename T,std::uint64_t p=100003,typename int_hash=linear_universal_hash_function<int,p>>
class poly_hash : public abstract_hash_function<T>
{
private:
	int_hash h;
	std::uint64_t x;
public:
	poly_hash(int m,int seed):abstract_hash_function<T>(m),h(m,seed)
	{
		std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<std::uint64_t> distrib1(1, p - 1);
		x = distrib1(gen);
	}
	poly_hash(int m) :abstract_hash_function<T>(m), h(m) 
	{

		std::random_device rd;
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<std::uint64_t> distrib1(1, p - 1);
		x = distrib1(gen);
	}
	std::uint64_t apply(T u) const
	{
		std::uint64_t hash(0);
		for (int i = u.size()-1; i >= 0; i--)
			hash = (x * hash + u[i])%p;
		return h(hash);
	}

	std::uint64_t apply_poly(T u) const
	{
		std::uint64_t hash(0);
		for (int i = u.size() - 1; i >= 0; i--)
			hash = (x * hash + u[i]) % p;
		return hash;
	}

	std::uint64_t X()const
	{
		return x;
	}
};