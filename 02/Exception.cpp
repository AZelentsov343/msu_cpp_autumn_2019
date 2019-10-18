#include <iostream>
#include <cstring>
#include "Exception.h"

Exception::Exception(const char *mes) {
    message = strcpy(message, mes);
}

void Exception::print_message() const {
    std::cerr << message << std::endl;
}