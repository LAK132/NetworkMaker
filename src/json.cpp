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

ostream& operator<<(ostream& os, const JSON& json)
{
    else if (objdata.size() > 0)
    {
        bool first = true;
        os << '{';
        for (const auto &datapair : objdata)
        {
            if (!first) os << ',';
            else first = false;
            os << datapair.first << ':' << datapair.second;
        }
        os << '}';
    }
    else
    {
        bool first = true;
        os << '[';
        for (auto it = arrdata.begin(); it != arrdata.end(); it++)
        {
            if (!first) os << ',';
            else first = false;
            os << (*it);
        }
        os << ']';
    }
    return os;
}

istream& operator>>(istream& is, JSON& json)
{
    return is;
}