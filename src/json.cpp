#include "json.hpp"

JSON::JSON() {
    objdata = map<string, JSON>();
    arrdata = vector<JSON>();
    data = vector<uint8_t>();
    //clear();
}

JSON::JSON(const string& str) : JSON() {
    if (str.length() > 0)
    {
        istringstream st(str);
        st >> *this;
    }
}

void JSON::clear() {
    objdata.clear();
    arrdata.clear();
    data.clear();
}

JSON& JSON::operator[](size_t idx) {
    return arrdata[idx];
}

JSON& JSON::at(size_t idx) {
    resize(idx);
    return (*this)[idx];
}

JSON& JSON::operator()(const string& idx) {
    try {
        return objdata.at(idx);
    }
    catch (const out_of_range& oor) {
        objdata.emplace(idx, JSON());
    }
    return objdata[idx];
}

void JSON::push_back(JSON&& json) {
    push_back(json);
}

void JSON::push_back(JSON& json) {
    arrdata.push_back(json);
}

void JSON::resize(size_t idx) {
    while(arrSize() <= idx)
    {
        push_back(JSON());
    }
}

vector<uint8_t>& JSON::operator->() {
    return data;
}

size_t JSON::objSize() {
    return objdata.size();
}

size_t JSON::arrSize() {
    return arrdata.size();
}

size_t JSON::size() {
    if(objSize() > 0) {return objSize();}
    else if(arrSize() > 0) {return arrSize();}
    else {return data.size();}
}

bool JSON::has(const string& idx){
    try {
        objdata.at(idx);
    }
    catch (const out_of_range& oor) {
        return false;
    }
    return true;
}

string& stringify(string& str) {
    //cout << "2 1 1 4 1" << endl;
    regex r("[\"\'\\0\\n\\t\\r\\\\]");
    //cout << "2 1 1 4 2" << endl;
    smatch sm;
    while(regex_search(str, sm, r))
    {
        //cout << "2 1 1 4 3" << endl;
        string ins;
        char c = sm[0].str()[0];
        //cout << "2 1 1 4 4" << endl;
        switch(c)
        {
            case '\"':
                ins = "\\\"";
                break;
            case '\'':
                ins = "\\\'";
                break;
            case '\0':
                ins = "\\0";
                break;
            case '\n':
                ins = "\\n";
                break;
            case '\t':
                ins = "\\t";
                break;
            case '\r':
                ins = "\\r";
                break;
            case '\\':
                ins = "\\\\";
                break;
        }
        str.erase(sm.position(),1);
        str.insert(sm.position(),ins);
        //cout << "2 1 1 4 5" << endl;
    }

    return str;
}

string& parse(string& str) {
    string temp = str;
    str = "";
    regex r("(?:[\\\\]([^\\\\]))");
    smatch sm;

    bool set = false;
    while(regex_search(temp, sm, r))
    {
        set = true;
        str += sm.prefix();
        string ins;
        char c = sm[0].str()[0];
        switch(c)
        {
            case '0':
                ins = "\0";
                break;
            case 'n':
                ins = "\n";
                break;
            case 't':
                ins = "\t";
                break;
            case 'r':
                ins = "\r";
                break;
            default:
                ins = c;
        }
        str += ins;
        temp = sm.suffix();
    }
    if(!set) str = temp;

    return str;
}

ostream& operator<<(ostream& os, const JSON& json) {
    //cout << "2 1\n" << json.objdata.size() << std::flush << endl;
    if (json.objdata.size() > 0)
    {
        //cout << "2 2\n";
        bool first = true;
        os << endl << '{' << endl;
        for (const auto &datapair : json.objdata)
        {
            if (!first) os << ',' << endl;
            else first = false;
            string s = datapair.first;
            os << "\"" << stringify(s) << "\" : " << datapair.second << " ";
        }
        os << endl << '}';
    }
    else if (json.arrdata.size() > 0)
    {
        //cout << "2 3\n";
        bool first = true;
        os << endl << '[' << endl;
        for (auto it = json.arrdata.begin(); it != json.arrdata.end(); it++)
        {
            if (!first) os << ',' << endl;
            else first = false;
            os << " " << (*it) << " ";
        }
        os << endl << ']';
    }
    else
    {
        //cout << "2 4\n";
        vector<uint8_t> s = stringifyv(json.data);
        os << "\"";
        for(auto it = s.begin(); it != s.end(); it++) os << (char)(*it);
        os << "\"";
    }
    return os;
}

char skipto(istream& is, string delim) {
    bool found = false;
    char c = 0;
    while(!found)
    {
        c = is.peek();
        for(size_t i = 0; i < delim.length(); i++)
        {
            if(delim[i] == c) found = true;
        }
        if(!found) is.get();
    }
    //cout << c;
    return c;
}

char skipover(istream& is, string delim) {
    bool found = false;
    char c = 0;
    while(!found)
    {
        c = is.get();
        for(size_t i = 0; i < delim.length(); i++)
        {
            if(delim[i] == c) found = true;
        }
    }
    //cout << c;
    return c;
}

istream& operator>>(istream& is, JSON& json) {
    //cout << "1 1" << endl;
    json.objdata.clear();
    json.arrdata.clear();
    char c = skipto(is, "\"[{");
    if(c != '"') c = is.get();
    //cout << "1 1 A " << c << endl;
    if (c == '{')
    {
        //cout << "{" << endl;
        while(c != '}')
        {
            skipover(is, "\"");
            c = is.get();
            string name = "";
            bool esc = false;
            while(c != '"' || esc)
            {
                //cout << "!esc: " << c;
                name += c;
                esc = (c == '\\') && !esc;
                c = is.get();
            }
            //cout << "name:" << name << endl;
            name = parse(name);
            json.objdata.emplace(name, JSON());

            skipover(is, ":");

            is >> json.objdata[name];
            //cout << "Name: " << name << " JSON: " << json.objdata[name] << endl;

            c = skipover(is, ",}");
        }
        //cout << "}" << endl;
    }
    else if (c == '[')
    {
        //cout << "[" << endl;
        while(c != ']')
        {
            skipto(is, "\"{[");

            json.arrdata.push_back(JSON());
            is >> json.arrdata.back();

            c = skipover(is, ",]");
        }
        //cout << "]" << endl;
    }
    else //this is actual data
    {
        //cout << "\"" << endl;
        skipover(is, "\"");

        c = is.get();
        vector<uint8_t> str(0, 0);
        bool esc = false;
        while(c != '"' || esc)
        {
            str.push_back(c);
            esc = (c == '\\') && !esc;
            c = is.get();
        }

        json.data = parsev(str);
        //cout << "\"" << endl;
    }
    return is;
}
