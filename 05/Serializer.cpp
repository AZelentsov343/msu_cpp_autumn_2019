#include <iostream>
#include <cctype>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream* a) : out(a) {}

    template <class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    std::ostream* out;

    template <class T, class T2, class... ArgsT>
    Error process(T first, T2 second, ArgsT... args) {
        handler(first);
        if (sizeof...(args) == 0) {
            return process(second);
        } else {
            return process(second, args...);
        }
    }

    template <class Last>
    Error process(Last last) {
        handler(last);
        return Error::NoError;
    }

    void handler(bool a) {
        *out << (a ? "true" : "false") << Separator;
    }

    void handler(uint64_t a) {
        *out << a << Separator;
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream* a) : in(a) {}

    template <class T>
    Error load(T &object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }

private:
    std::istream* in;

    template <class T, class T2, class... ArgsT>
    Error process(T& first, T2& second, ArgsT&... args) {
        int res = handler(first);
        if (res) {
            return Error::CorruptedArchive;
        }
        if (sizeof...(args) == 0) {
            return process(second);
        } else {
            return process(second, args...);
        }
    }

    template <class Last>
    Error process(Last& last) {
        int res = handler(last);
        if (res) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    int handler(bool& a) {
        std::string text;
        *in >> text;
        if (text == "true") {
            a = true;
        } else if (text == "false") {
            a = false;
        } else {
            return 1;
        }
        return 0;
    }

    int handler(uint64_t& a) {
        std::string text;
        *in >> text;
        try {
            if (!isdigit(text[0])) {
                return 1;
            }
            a = std::stoull(text);
            return 0;
        } catch (...) {
            return 1;
        }
    }
};
