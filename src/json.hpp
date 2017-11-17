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
#include "bytecount.hpp"
using btc::bytecount;
using btc::bytecpy;
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
	JSON();
	JSON(const string& str);
	void clear();
	//Access operators
	JSON& operator[](size_t idx);
	JSON& at(size_t idx);
	JSON& operator()(const string& idx);
	//Allocation operators
	void push_back(JSON&& json);
	void push_back(JSON& json);
	void resize(size_t idx);	//does not shrink arrdata

	vector<uint8_t>& operator->();
	//Size operators
	size_t objSize();
	size_t arrSize();
	size_t size();
	bool has(const string& idx);
	//IO operators
    friend ostream& operator<<(ostream& os, const JSON& json);
	friend istream& operator>>(istream& is, JSON& json);
	//Value access operators
	template<typename T>
	T get() {
		T t;
		//if (data.size() < bytecount(T)) return t;
		//memcpy(&t, &(data[0]), bytecount(T));
		bytecpy(t, data);
		return t;
	}
	template<typename T>
	void set(const T&& t) {
		set(t);
	}
	template<typename T>
	void set(const T& t) {
		//data.resize(bytecount(T));
		//memcpy(&(data[0]), &t, sizeof(T));
		bytecpy(data, t);
	}
	template<typename T>
	const T& init(const string& idx, const T&& t){
        return init(idx, t);
	}
	template<typename T>
	const T& init(const string& idx, const T& t){      //Init idx to t if it not already set
        return ((*this).has(idx) ? (*this)(idx).template get<T>() : t);
	}
};

#endif
