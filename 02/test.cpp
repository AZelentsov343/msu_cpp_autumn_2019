#include <iostream>
#include <cstring>
#include <chrono>
#include "LinearAllocator.h"
#include "Exception.h"
#include "Timer.h"

//Func main is used only to test LinearAllocator
int main() {
    using namespace std;

    auto la = LinearAllocator(4);
    char *ptr1 = la.alloc(2);
    if (ptr1 == nullptr) {
        cerr << "Error: allocator cannot allocate memory" << endl;
        return 1;
    }
    ptr1[0] = 'a';
    ptr1[1] = '\0';
    if (strcmp(ptr1, "a") != 0) {
        cerr << "Error: cannot use allocated memory" << endl;
        return 1;
    }
    char *ptr2 = la.alloc(4);
    if (ptr2 != nullptr) {
        cerr << "Error: allocator still allocates memory after overflow" << endl;
        return 1;
    }
    la.reset();
    char *ptr3 = la.alloc(1);
    if (ptr1 != ptr3) {
        cerr << "Error: allocator does not reset correct" << endl;
        return 1;
    }
    la.reset();
    char *ptr4 = la.alloc(10);
    if (ptr4 != nullptr) {
        cerr << "Error: allocator still allocates memory after reset and overflow" << endl;
        return 1;
    }
    Timer t;
    t.start();
    char *ptr5 = (char *)malloc(4);
    long long res1 = t.finish();
    t.start();
    char *ptr6 = la.alloc(4);
    long long res2 = t.finish();
    if (res2 > res1) {
        cerr << "Allocator works much slower than it supposed to" << endl;
        return 1;
    }
    cout << "Everything OK" << endl;
    return 0;
}

