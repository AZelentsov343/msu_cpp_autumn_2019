#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <vector>
#include <chrono>
#include <cstdio>
#include "sort.h"

using namespace std;

void generate_file(const string &filename, const string &test_filename, size_t size) {
    srand(time(NULL));
    ofstream test_fout(test_filename, ios::binary | ios::out);
    ofstream fout(filename, ios::binary | ios::out);
    for (size_t i = 0; i < size; ++i) {
        uint64_t tmp = rand();
        fout.write((char *) &tmp, sizeof(tmp));
        test_fout.write((char *) &tmp, sizeof(tmp));
    }
}

int main() {
    generate_file("f1.bin", "f2.bin", 12345678);
    my_sort("f1.bin", "out.bin");
    ifstream fin("f2.bin", ios::binary | ios::in);
    ifstream fout("out.bin", ios::binary | ios::in);
    vector<uint64_t> v;
    uint64_t tmp;
    while (true) {
        if (not fin.read((char *) &tmp, sizeof(tmp))) {
            break;
        }
        v.push_back(tmp);
    }
    sort(v.begin(), v.end());
    for (uint64_t i : v) {
        if (fout.read((char *) &tmp, sizeof(tmp))) {
            if (tmp != i) {
                cout << "Fail1!" << endl;
                return 0;
            }
        } else {
            cout << "Fail2!" << endl;
            return 0;
        }
    }
    fin.close();
    fout.close();
    remove("f1.bin");
    remove("f2.bin");
    remove("out.bin");
    cout << "done" << endl;
    return 0;
}