#ifndef INC_09_SORT_H
#define INC_09_SORT_H

#include <cstdio>
#include <string>
#include <mutex>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#define MAX 1024 * 128
using namespace std;

mutex mutex_count;
size_t count_files = 0;

size_t get_file_size(std::string filename) {
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file, 0, SEEK_END);
    size_t size = ftell(p_file) / sizeof(uint64_t);
    fclose(p_file);
    return size;
}

void sort_one_file(ifstream &file, mutex &m) {
    vector<uint64_t> nums;
    nums.reserve(MAX);
    while (1) {
        nums.clear();
        for (int i = 0; i < MAX; i++) {
            uint64_t num;
            unique_lock<mutex> u(m);
            if (not file.read((char *)&num, sizeof(uint64_t))) {
                break;
            } else {
                u.unlock();
                nums.push_back(num);
            }
            if (nums.empty()) {
                return;
            } else {
                mutex_count.lock();
                count_files++;
                mutex_count.unlock();
            }
        }
        sort(nums.begin(), nums.end());
        stringstream a;
        mutex_count.lock();
        a << "file" << count_files << ".bin";
        mutex_count.unlock();
        ofstream file_count(a.str(), ios::binary | ios::out);
        for (auto num : nums) {
            file_count.write((char*)&num, sizeof(num));
        }
    }

}

void merge(ifstream& first, off_t size1, ifstream& second, off_t size2, ofstream& result) {
    result.seekp(0);
    uint64_t* arr1 = new uint64_t[size1];
    uint64_t* arr2 = new uint64_t[size2];

    first.seekg(0);
    first.read((char*)arr1, sizeof(uint64_t) * size1);

    second.seekg(0);
    second.read((char*)arr2, sizeof(uint64_t) * size2);

    off_t i = 0, j = 0;

    while (i < size1 && j < size2) {
        if (arr1[i] < arr2[j]) {
            result.write((char *) (arr1 + i), sizeof(uint64_t));
            i++;
        } else {
            result.write((char *) (arr2 + j), sizeof(uint64_t));
            j++;
        }
    }

    while (i < size1) {
        result.write((char *) (arr1 + i), sizeof(uint64_t));
        i++;
    }

    while (j < size2) {
        result.write((char *) (arr2 + j), sizeof(uint64_t));
        j++;
    }
    delete[] arr1;
    delete[] arr2;
}


void my_sort(const string& file_in, const string& file_out) {
    count_files = 0;
    vector<string> v;

    ifstream fin(file_in, ios::binary | ios::in);
    mutex m;

    thread t1(sort_one_file, ref(fin), ref(m));
    thread t2(sort_one_file, ref(fin), ref(m));
    t1.join();
    t2.join();
    fin.close();

    printf("here\n");
    fflush(stdout);
    for (int i = 0; i < count_files; i++) {
        stringstream sout;
        sout << "file" << i << ".bin";
        v.push_back(sout.str());
    }

    while (v.size() != 1) {
        auto f1 = v.back();
        v.pop_back();
        auto f2 = v.back();
        v.pop_back();
        ifstream fin1(f1, ios::binary | ios::in);
        ifstream fin2(f1, ios::binary | ios::in);
        auto size1 = get_file_size(f1);
        auto size2 = get_file_size(f2);
        count_files++;
        stringstream sout;
        sout << "file" << count_files << ".bin";
        v.push_back(sout.str());
        ofstream fout(sout.str(), ios::binary | ios::in);
        merge(fin1, size1, fin2, size2, fout);
    }

    auto size = get_file_size(v[0]);
    ifstream fin2(v[0], ios::binary | ios::in);
    ofstream fout(file_out, ios::binary | ios::in);
    for (int i = 0; i < size; i++) {
        uint64_t tmp;
        fin2.read((char *)&tmp, sizeof(uint64_t));
        fout.write((char *)&tmp, sizeof(uint64_t));
    }
}


#endif //INC_09_SORT_H
