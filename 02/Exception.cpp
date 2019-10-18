#include <iostream>
#include "Exception.h"

Exception::Exception(const char *mes) {
    message = mes;
}

void Exception::print_message() {
    std::cerr << message << std::endl;
}