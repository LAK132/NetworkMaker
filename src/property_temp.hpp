template<typename T> 
void del(void* p)
{
	delete (T*)p;
}

template<typename T, typename... Ta>
Property::Property(Ta... args)
{
    isInit = false;
    init<T>(args...);
}

template<typename T, typename... Ta>
void Property::init(Ta... args)
{
    if (isInit) deleter(value);
    deleter = del<T>;
    value = new T(args...);
    dsize = sizeof(T);
    psize = sizeof(T*);
    isInit = true;
}

template<typename T>
void Property::set(T val)
{
    if (!isInit) init<T>();
    *((T*)value) = val;
}

template<typename T>
void Property::setr(T& val)
{
    if (!isInit) init<T>();
    *((T*)value) = val;
}

template<typename T>
T Property::get()
{
    if (!isInit) init<T>();
    return *((T*)value);
}

template<typename T>
T& Property::getr()
{
    //if (!isInit) init<T>();
    return *((T*)value);
}

template<typename T>
T* Property::getp()
{
    if (!isInit) init<T>();
    return ((T*)value);
}