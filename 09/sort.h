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
#include <queue>

using namespace std;

enum {
    MAX = 1024 * 128
};


void sort_one_file(ifstream &fin, mutex &m, size_t &count_files, std::mutex &mutex_count) {
    auto *nums = new uint64_t[MAX]();
    size_t size = 0;
    size_t cur = 0;
    while (true) {
        size = 0;
        for (size_t i = 0; i < MAX; i++) {
            unique_lock <mutex> u(m);
            uint64_t num;
            if (!fin.read((char *) &num, sizeof(num))) {
                break;
            }
            u.unlock();
            nums[size++] = num;
        }
        if (size == 0) {
            delete[] nums;
            return;
        } else {
            std::lock_guard <std::mutex> guard(mutex_count);
            cur = count_files;
            count_files++;
        }
        sort(nums, nums + size);
        stringstream my_out;
        my_out << "file" << cur << ".bin";
        ofstream fout(my_out.str(), std::ios::binary | std::ios::out);
        for (size_t i = 0; i < size; i++) {
            fout.write((char *) &nums[i], sizeof(nums[i]));
        }
    }
}


void my_sort(const string &name_in, const string &name_out) {
    mutex m, num_mutex;
    ifstream file_in(name_in, ios::binary | ios::in);
    size_t parts = 0;

    thread t1(sort_one_file, ref(file_in), ref(m), std::ref(parts), ref(num_mutex));
    thread t2(sort_one_file, ref(file_in), ref(m), std::ref(parts), ref(num_mutex));
    t1.join();
    t2.join();
    file_in.close();

    vector <string> files;
    for (size_t i = 0; i < parts; ++i) {
        stringstream sout;
        sout << "file" << i << ".bin";
        files.push_back(sout.str());
    }

    ofstream file_out(name_out, ios::binary | ios::out);

    auto from = files.cbegin();
    auto till = files.cend();

    priority_queue < pair < uint64_t, size_t >, vector < pair < uint64_t, size_t >>, std::greater < pair < uint64_t,
            size_t >> > queue;
    size_t size = till - from;
    vector <ifstream> f_vect;

    for (vector<string>::const_iterator i = from; i != till; i++) {
        f_vect.emplace_back(*i, ios::binary | ios::in);
    }

    for (size_t i = 0; i < f_vect.size(); i++) {
        if (f_vect[i].is_open()) {
            uint64_t num;
            if (f_vect[i].read((char *) &num, sizeof(num))) {
                queue.push(make_pair(num, i));
            }
        }
    }

    while (!queue.empty()) {
        uint64_t tmp;
        size_t size;
        tie(tmp, size) = queue.top();
        queue.pop();
        file_out.write((char *) &tmp, sizeof(tmp));
        if (f_vect[size].read((char *) &tmp, sizeof(tmp))) {
            queue.push(make_pair(tmp, size));
        }
    }

    file_out.close();

    for (auto &file : files) {
        remove(file.c_str());
    }
}


#endif //INC_09_SORT_H
