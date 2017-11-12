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

vector<uint8_t> stringifyv(const vector<uint8_t>& str)
{
    vector<uint8_t> rtn(str.size()*2, 0);

    for(size_t i = 0, k = 0; i < str.size(); i++)
    {
        rtn[k++] = ((str[i] & 0xF0) >> 4) + '0';
        rtn[k++] = (str[i] & 0x0F) + '0';
    }

    return rtn;
}
vector<uint8_t> Property::getv()
{
    vector<uint8_t> str(dsize, 0);
    if(!isInit) return str;
    memcpy(&(str[0]), value, dsize);
    vector<uint8_t> rtn = stringifyv(str);
    return rtn;
}
vector<uint8_t> parsev(const vector<uint8_t>& str)
{
    vector<uint8_t> rtn(str.size()/2, 0);

    for(size_t i = 0, k = 0; i < rtn.size(); i++)
    {
        rtn[i] = ((((uint8_t)str[k++] - '0') << 4) & 0xF0) | 
                 (((uint8_t)str[k++] - '0') & 0x0F);
    }

    return rtn;
}
void Property::setv(const vector<uint8_t>& str)
{
    if(!isInit) return;
    vector<uint8_t> temp = parsev(str);
    memcpy(value, &(temp[0]), dsize);
}