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
        if (typeid(first) == typeid(uint64_t)) {
            *out << first << Separator;
        } else if (typeid(first) == typeid(bool)) {
            *out << (first ? "true" : "false") << Separator;
        } else {
            return Error::CorruptedArchive;
        }
        if (sizeof...(args) == 0) {
            return process(second);
        } else {
            return process(second, args...);
        }
    }

    template <class Last>
    Error process(Last last) {
        if (typeid(last) == typeid(uint64_t)) {
            *out << last;
        } else if (typeid(last) == typeid(bool)) {
            *out << (last ? "true" : "false");
        } else {
            return Error::CorruptedArchive;
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

    template <class T, class T2, class... ArgsT>
    Error process(T& first, T2& second, ArgsT&... args) {
        std::string text;
        *in >> text;
        std::string::iterator it = text.begin();
        std::string::iterator end = text.end();
        if (typeid(first) == typeid(uint64_t&)) {
            uint64_t tmp = 0;
            if (it == end) {
                return Error::CorruptedArchive;
            }
            if (!isdigit(*it)) {
                return Error::CorruptedArchive;
            }
            while (it != end && isdigit(*it)) {
                tmp *= 10;
                tmp += *it - '0';
                it++;
            }
            if (it != end) {
                return Error::CorruptedArchive;
            }
            first = tmp;
        } else if (typeid(first) == typeid(bool&)) {
            if (it == end) {
                return Error::CorruptedArchive;
            }
            if (*it == 't') {
                it++;
                if (*it != 'r') return Error::CorruptedArchive;
                it++;
                if (*it != 'u') return Error::CorruptedArchive;
                it++;
                if (*it != 'e') return Error::CorruptedArchive;
                first = true;
            } else if (*it == 'f') {
                it++;
                if (*it != 'a') return Error::CorruptedArchive;
                it++;
                if (*it != 'l') return Error::CorruptedArchive;
                it++;
                if (*it != 's') return Error::CorruptedArchive;
                it++;
                if (*it != 'e') return Error::CorruptedArchive;
                first = false;
            } else {
                return Error::CorruptedArchive;
            }
        } else {
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
        std::string text;
        *in >> text;
        std::string::iterator it = text.begin();
        std::string::iterator end = text.end();
        if (typeid(last) == typeid(uint64_t&)) {
            uint64_t tmp = 0;
            if (it == end) {
                return Error::CorruptedArchive;
            }
            if (!isdigit(*it)) {
                return Error::CorruptedArchive;
            }
            while (it != end && isdigit(*it)) {
                tmp *= 10;
                tmp += *it - '0';
                it++;
            }
            if (it != end) {
                return Error::CorruptedArchive;
            }
            last = tmp;
        } else if (typeid(last) == typeid(bool&)) {
            if (it == end) {
                return Error::CorruptedArchive;
            }
            if (*it == 't') {
                it++;
                if (*it != 'r') return Error::CorruptedArchive;
                it++;
                if (*it != 'u') return Error::CorruptedArchive;
                it++;
                if (*it != 'e') return Error::CorruptedArchive;
                last = true;
            } else if (*it == 'f') {
                it++;
                if (*it != 'a') return Error::CorruptedArchive;
                it++;
                if (*it != 'l') return Error::CorruptedArchive;
                it++;
                if (*it != 's') return Error::CorruptedArchive;
                it++;
                if (*it != 'e') return Error::CorruptedArchive;
                last = false;
            } else {
                return Error::CorruptedArchive;
            }
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};
