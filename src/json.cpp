#include "json.hpp"

JSON& JSON::operator[](size_t idx)       
{ 
    return arrdata[idx]; 
}
JSON& JSON::operator()(const string& idx)       
{ 
    return objdata[idx]; 
}
Property* JSON::operator->()
{
    return &data;
}
size_t JSON::objSize()
{
    return objdata.size();
}
size_t JSON::arrSize()
{
    return arrdata.size();
}