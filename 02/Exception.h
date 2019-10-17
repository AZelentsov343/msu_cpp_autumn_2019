#ifndef INC_02_EXCEPTION_H
#define INC_02_EXCEPTION_H


class Exception {
public:
    Exception(std::string mes);
    void print_message();

private:
    const std::string message;
};


#endif //INC_02_EXCEPTION_H
