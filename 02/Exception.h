#ifndef DESKTOP_EXCEPTION_H
#define DESKTOP_EXCEPTION_H

class Exception {
public:
    Exception(const char *mes);
    ~Exception();
    void print_message() const;
private:
    char *message;
};


#endif //DESKTOP_EXCEPTION_H
