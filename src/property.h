//https://stackoverflow.com/questions/13980157/c-class-with-template-member-variable
#ifndef PROPERTY_H
#define PROPERTY_H

class Property;

template<typename T>
void del(void* p)
{
	delete (T*)p;
}

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
	template<typename T>
	void init()
	{
		deleter = del<T>;
		value = new T();
		isInit = true;
	}
	template<typename T>
	Property()
	{
		init<T>();
	}
	~Property()
	{
		if (isInit) deleter(value);
	}
	template<typename T>
	void set(T val)
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
};
#endif