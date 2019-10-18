#include <iostream>
#include "Exception.h"


Exception::Exception(const std::string& mes) {
    message = mes;
}

void Exception::print_message() const {
    std::cerr << message << std::endl;
}