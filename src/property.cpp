#include "property.hpp"

void noDel(void* p) {}

Property::Property()
{
    isInit = false;
}

Property::~Property()
{
    if (isInit) deleter(value);
}

size_t Property::size()
{
    return dsize;
}