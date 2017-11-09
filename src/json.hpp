#include <stdlib.h>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <iostream>
using std::istream;
using std::ostream;
#include "property.hpp"

#ifndef JSON_H
#define JSON_H

class JSON
{
private:
	map<string, JSON> objdata;
	vector<JSON> arrdata;
	Property data;
public:
	JSON& operator[](size_t idx);
	JSON& operator()(const string& idx);
	Property* operator->();
	size_t objSize();
    size_t arrSize();
    friend ostream& operator<<(ostream& os, const JSON& json);
    friend istream& operator>>(istream& is, JSON& json);
};

#endif