#pragma once
#include <cctype>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

template <class T>
string process(string& text, vector<string>& v, T&& arg) {
    stringstream s;
    s << arg;
    v.push_back(s.str());
    stringstream new_text;
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '{') {
            if (text[i + 1] == '}') {
                throw runtime_error("expected value");
            }
            size_t pos = i + 1;
            size_t substr_size = 1;
            while (text[i + substr_size] != '}') {
                substr_size++;
                if (i + substr_size - 1 == text.size() or text[i + substr_size - 1] == '{' or !isdigit(text[i + substr_size - 1])) {
                    throw runtime_error("expected closing scope");
                }
            }
           substr_size--;
            try {
                auto l = text.substr(pos, substr_size);
                auto num = stoull(l);
                if (num >= v.size()) {
                    throw runtime_error("out of range");
                }
                new_text << v[num];
                i = pos + substr_size;
            }
            catch (out_of_range& e) {
                throw runtime_error("out of range");
            }
            catch (invalid_argument& e) {
                throw runtime_error("invalid_argument");
            }
        } else if (text[i] == '}') {
            throw runtime_error("expected opening scope before closing");
        } else {
            new_text << text[i];
        }
    }
    return new_text.str();
}

template <class T, class... Args>
string process(string& text, vector<string>& v, T&& arg, Args&&... args) {
    stringstream s;
    s << arg;
    v.push_back(s.str());
    return process(text, v, forward<Args>(args)...);
}

string process(string& text, vector<string>& v) {
    return text;
}


template <class... Args>
string format(const char *text, Args&&... args) {
    auto new_text = string(text);
    vector<string> v;
    return process(new_text, v, forward<Args>(args)...);
}




