#pragma once
#include <string>
#include <vector>
#include <sstream>

struct Error {
    std::string message;
    Error (const std::string& mes): message(mes) {}
    std::string what() const {
        return message;
    }
};

class Base {
    public:
        Base () {}
        virtual ~Base() {}
        virtual void to_stream(std::ostringstream &) = 0;
};

template <class T>
class Arg: public Base {
    public:
        Arg (const T& a): arg(a) {}
        virtual ~Arg () {}
        virtual void to_stream(std::ostringstream & out) {
            out << arg;
        }

    private:
        T arg;
};

struct ArgsList {
    ArgsList() {}
    ~ArgsList() {
            for (auto i: args)
                delete i;
        }
    
    std::vector<Base *> args;
};

template <class T>
void process(ArgsList& arg_list, T&& arg) {
    arg_list.args.push_back(new Arg<T>(arg));
}

template <class T, class... Args>
void process(ArgsList& args_list, T&& arg, Args&&... args) {
    process(args_list, arg);
    process(args_list, std::forward<Args>(args)...);
}

std::string format(const std::string & str) {
    for (auto i: str)
        if (i == '{' || i == '}') throw Error("Unexpected bracket");
    return str;
}

template <class... Args>
std::string format(const std::string & str, Args&&... args) {
    ArgsList args_list;
    process(args_list, std::forward<Args>(args)...);

    std::ostringstream res;
    size_t sz = str.size();
    bool flag = false;
    std::string index;
    std::string buffer;

    for (size_t i = 0; i < sz; ++i) {
        if (flag) { // внутри скобок
            if (str[i] == '}') {
                try
                {
                    auto num = std::stoull(index);
                    if (num >= args_list.args.size())
                        throw std::logic_error("");
                    args_list.args[num] -> to_stream(res);
                    flag = false;
                    continue;
                }
                catch(const std::logic_error& e)
                {
                    throw Error("Wrong value inside {}");
                }
                
            }

            if (str[i] == '{') {
                throw Error("Double {");
            }

            index += str[i];
            continue;
        }
        // вне скобок
        if (str[i] == '{') {
            res << buffer;
            buffer = "";
            flag = true;
            index = "";
            continue;
        }
        if (str[i] == '}') {
            throw Error("Unexpected }");
        }
        buffer += str[i];
    }
    if (flag) {
        throw Error("Expected }");
    }
    if (buffer != "") res << buffer;

    return res.str();
}
