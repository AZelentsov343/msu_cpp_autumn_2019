#include <iostream>
#include "calc.h"

using namespace std;

int main(int argc1 , char *argv[]) {
    normalize_str(argv[1]);
    cout << calc(argv[1]) << endl;
    return 0;
}