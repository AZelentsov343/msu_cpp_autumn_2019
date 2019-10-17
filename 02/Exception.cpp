#include <iostream>
#include "Exception.h"

Exception::Exception(std::string mes) {
    message = mes;
}

void Exception::print_message() {
    std::cerr << message << endl;
}