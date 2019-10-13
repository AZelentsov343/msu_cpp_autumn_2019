#include <iostream>
#include "calc.h"

using namespace std;

int main(int argc , char *argv[]) {
    if (argc != 2) {
        cout << "error" << endl;
        exit(1);
    }
    cout << calc(argv[1]) << endl;
    return 0;
}