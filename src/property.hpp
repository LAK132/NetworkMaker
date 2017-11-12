#include <stdlib.h>
#include <string>
#include <cstring>
using std::memcpy;
#include <vector>
using std::vector;

#ifndef PROPERTY_H
#define PROPERTY_H

vector<uint8_t> stringifyv(const vector<uint8_t>& str);
vector<uint8_t> parsev(const vector<uint8_t>& str);

class Property
{
private:
	size_t dsize = 0;
	bool isInit = false;
	void* value;
	void(*deleter)(void*);
public:
	Property();
	~Property();
	template<typename T, typename... Ta>
	Property(Ta... args);
	template<typename T, typename... Ta>
	void init(Ta... args);
	size_t size();
	template<typename T>
	void set(T val);
	template<typename T>
	void setr(T& val);
	template<typename T>
	T get();
	template<typename T>
	T& getr();
	template<typename T>
	T* getp();
	vector<uint8_t> getv();
	void setv(const vector<uint8_t>& str);
};

#include "property_temp.hpp"

#endif