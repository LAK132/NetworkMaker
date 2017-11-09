#include <stdlib.h>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
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
};

#endif