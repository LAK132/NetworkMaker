#include <stdlib.h>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <iostream>
using std::istream;
using std::istringstream;
using std::ostream;
#include <regex>
using std::regex;
using std::smatch;
using std::regex_search;
#include <stdio.h>
using std::cin;
using std::cout;
using std::endl;
#include <stdexcept>
using std::out_of_range;
#include "property.hpp"

#ifndef JSON_H
#define JSON_H

string& stringify(string& str);
string& parse(string& str);

class JSON
{
private:
	map<string, JSON> objdata;
	vector<JSON> arrdata;
	vector<uint8_t> data;
public:
	JSON(const string& str = "");
	JSON& operator[](size_t idx);
	void push_back(JSON&& json);
	void push_back(JSON& json);
	JSON& operator()(const string& idx);
	vector<uint8_t>& operator->();
	size_t objSize();
	size_t arrSize();
	size_t size();
    friend ostream& operator<<(ostream& os, const JSON& json);
	friend istream& operator>>(istream& is, JSON& json);
	template<typename T>
	T get()
	{
		T t;
		memcpy(&t, &(data[0]), sizeof(T));
		return t;
	}
	template<typename T>
	void set(const T&& t)
	{
		set(t);
	}
	template<typename T>
	void set(const T& t)
	{
		data.resize(sizeof(T));
		memcpy(&(data[0]), &t, sizeof(T));
	}
};

#endif