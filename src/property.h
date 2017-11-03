//https://stackoverflow.com/questions/13980157/c-class-with-template-member-variable
#ifndef PROPERTY_H
#define PROPERTY_H

template<typename T> 
void del(void* p)
{
	delete (T*)p;
}

void noDel(void* p) {}

class Property
{
private:
	bool isInit;
	void* value;
	void(*deleter)(void*);
public:
	Property()
	{
		isInit = false;
	}
	~Property()
	{
		if (isInit) deleter(value);
	}
	template<typename T, typename... Ta>
	Property(Ta... args)
	{
		init<T>(args...);
	}
	template<typename T, typename... Ta>
	void init(Ta... args)
	{
		if (isInit) deleter(value);
		deleter = del<T>;
		//value = (T*)malloc(sizeof(T));
		value = new T(args...);
		isInit = true;
	}
	template<typename T>
	void set(T val)
	{
		if (!isInit) init<T>();
		*((T*)value) = val;
	}
	template<typename T>
	void setr(T& val)
	{
		if (!isInit) init<T>();
		*((T*)value) = val;
	}
	template<typename T>
	T get()
	{
		if (!isInit) init<T>();
		return *((T*)value);
	}
	template<typename T>
	T& getr()
	{
		//if (!isInit) init<T>();
		return *((T*)value);
	}
	template<typename T>
	T* getp()
	{
		if (!isInit) init<T>();
		return ((T*)value);
	}
};

#endif