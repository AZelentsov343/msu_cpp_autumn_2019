#ifndef INC_05_SERIALIZER_H
#define INC_05_SERIALIZER_H

#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream* a);

    template <class T>
    Error save(T& object);

    template <class... ArgsT>
    Error operator()(ArgsT... args);

private:
    std::ostream* out;
    template <class... ArgsT>
    Error process(ArgsT... args);
};

class Deserializer {
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream* a);

    template <class T>
    Error load(T& object);

    template <class... ArgsT>
    Error operator()(ArgsT&... args);

private:
    std::istream* in;

    template <class... ArgsT>
    Error process(ArgsT&... args);
};

#endif //INC_05_SERIALIZER_H
