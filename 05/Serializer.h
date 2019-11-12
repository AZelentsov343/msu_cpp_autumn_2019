#pragma once
#include <iostream>
#include <cctype>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& a) : out(a) {}

    template <class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    std::ostream& out;

    template <class T, class... ArgsT>
    Error process(T&& first, ArgsT&&... args) {
        handler(first);
        return process(std::forward<ArgsT>(args)...);
    }

    template <class Last>
    Error process(Last&& last) {
        handler(last);
        return Error::NoError;
    }

    void handler(bool a) {
        out << (a ? "true" : "false") << Separator;
    }

    void handler(uint64_t a) {
        out << a << Separator;
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& a) : in(a) {}

    template <class T>
    Error load(T &object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }

private:
    std::istream& in;

    template <class T, class... ArgsT>
    Error process(T&& first, ArgsT&&... args) {
        auto res = handler(first);
        if (res == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    template <class Last>
    Error process(Last&& last) {
        return handler(last);
    }

    Error handler(bool& a) {
        std::string text;
        in >> text;
        if (text == "true") {
            a = true;
        } else if (text == "false") {
            a = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error handler(uint64_t& a) {
        std::string text;
        in >> text;
        try {
            if (!isdigit(text[0])) {
                return Error::CorruptedArchive;
            }
            a = std::stoull(text);
            return Error::NoError;
        } catch (const std::exception &e) {
            return Error::CorruptedArchive;
        }
    }
};
