//https://stackoverflow.com/questions/13980157/c-class-with-template-member-variable
#include <stdlib.h>

#ifndef PROPERTY_H
#define PROPERTY_H

class Property
{
private:
	size_t dsize;
	size_t psize;
	bool isInit;
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
};

#include "property_temp.hpp"

#endif