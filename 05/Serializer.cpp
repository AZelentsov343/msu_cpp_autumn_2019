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

    template <class... ArgsT>
    Error process(ArgsT... args) {
        for (auto i : {args...}) {
            if (typeid(i) == typeid(uint64_t)) {
                *out << i << Separator;
            } else if (typeid(i) == typeid(bool)) {
                *out << (i ? "true" : "false") << Separator;
            } else {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
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

    template <class... ArgsT>
    Error process(ArgsT&... args) {
        std::string text;
        *in >> text;
        std::string::iterator it = text.begin();
        for (auto& i: {args...}) {
            if (typeid(i) == typeid(uint64_t&)) {
                uint64_t tmp = 0;
                if (it == text.end()) {
                    return Error::CorruptedArchive;
                }
                if (!isdigit(*it)) {
                    return Error::CorruptedArchive;
                }
                while (it != text.end() && isdigit(*it)) {
                    tmp *= 10;
                    tmp += *it - '0';
                    it++;
                }
                if (*it == Separator) {
                    it++;
                    i = tmp;
                } else if (it != text.end()) {
                    return Error::CorruptedArchive;
                }
            } else if (typeid(i) == typeid(bool&)) {
                if (it == text.end()) {
                    return Error::CorruptedArchive;
                }
                if (*it == 't') {
                    it++;
                    if (*it != 'r') return Error::CorruptedArchive;
                    it++;
                    if (*it != 'u') return Error::CorruptedArchive;
                    it++;
                    if (*it != 'e') return Error::CorruptedArchive;
                    i = true;
                } else if (*it == 'f') {
                    it++;
                    if (*it != 'a') return Error::CorruptedArchive;
                    it++;
                    if (*it != 'l') return Error::CorruptedArchive;
                    it++;
                    if (*it != 's') return Error::CorruptedArchive;
                    it++;
                    if (*it != 'e') return Error::CorruptedArchive;
                    i = false;
                } else {
                    return Error::CorruptedArchive;
                }
            } else {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
    }
};
