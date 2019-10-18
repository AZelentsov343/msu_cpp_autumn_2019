#include <cstring>
#include <cstdlib>
#include <iostream>
#include "Exception.h"


Exception::Exception(const char *mes) {
    message = (char *) malloc(sizeof(*mes) * strlen(mes));
    strcpy(message, mes);
}

Exception::~Exception() {
    free(message);
}

void Exception::print_message() const {
    std::cerr << message << std::endl;
}