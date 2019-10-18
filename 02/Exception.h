#ifndef DESKTOP_EXCEPTION_H
#define DESKTOP_EXCEPTION_H
#include <string>

class Exception {
public:
    Exception(const std::string& mes);

    void print_message() const;

private:
    std::string message;
};


#endif //DESKTOP_EXCEPTION_H
