#pragma once
#include <string>
#include <vector>
#include <sstream>

class Base {
    public:
        Base () {}
        virtual ~Base() {}
        virtual void to_stream(std::ostringstream &) = 0;
};

template <class T>
class Arg: public Base {
    public:
        Arg (T a): arg(a) {}
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
void process(ArgsList& arg_list, T arg) {
    arg_list.args.push_back(new Arg<T>(arg));
}

template <class T, class... Args>
void process(ArgsList& args_list, T arg, Args&&... args) {
    process(args_list, arg);
    process(args_list, std::forward<Args>(args)...);
}

std::string format(const std::string & str) {
    for (auto i: str)
        if (i == '{' || i == '}') throw std::runtime_error("Unexpected bracket");
    return str;
}

template <class... Args>
std::string format(const std::string & str, Args&&... args) {
    ArgsList args_list;
    process(args_list, std::forward<Args>(args)...);

    std::ostringstream res;
    size_t sz = str.size();
    bool flag = false;
    std::string buffer;
    std:: string buffer1;

    for (size_t i = 0; i < sz; ++i) {
        if (flag) { // внутри скобок
            if (str[i] == '}') {
                try
                {
                    auto num = std::stoull(buffer);
                    if (num >= args_list.args.size())
                        throw std::logic_error("");
                    args_list.args[num] -> to_stream(res);
                    flag = false;
                    continue;
                }
                catch(const std::logic_error& e)
                {
                    throw std::runtime_error("Wrong value inside {}");
                }
                
            }

            if (str[i] == '{') {
                throw std::runtime_error("Double {");
            }

            buffer += str[i];
            continue;
        }
        // вне скобок
        if (str[i] == '{') {
            res << buffer1;
            buffer1 = "";
            flag = true;
            buffer = "";
            continue;
        }
        if (str[i] == '}') {
            throw std::runtime_error("Unexpected }");
        }
        buffer1 += str[i];
    }
    if (flag) {
        throw std::runtime_error("Expected }");
    }
    if (buffer1 != "") res << buffer1;

    return res.str();
}
