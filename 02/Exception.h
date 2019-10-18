#ifndef INC_02_EXCEPTION_H
#define INC_02_EXCEPTION_H


class Exception {
public:
    Exception(const char *mes);

    void print_message();

private:
    const char *message;
};


#endif //INC_02_EXCEPTION_H
