#include "TokenParser.h"
#include <sstream>
#include <cctype>

void TokenParser::Parse(const std::string & text) {
    std::string token;
    bool is_num = true;
    std::stringstream ss(text);
    RunHandlers(start, text);
    while (ss >> token) {
        for (auto l: token)
            if (!isdigit(l)) {
                is_num = false;
                break;
            }
        RunHandlers(anytok, token);
        if (is_num) 
            RunHandlers(digtok, token);
        else 
            RunHandlers(strtok, token);
        is_num = true;
    }
    RunHandlers(end, text);
}

void TokenParser::RunHandlers(const std::vector<HandlerFunc> &callback,
                              const std::string &token) {
    for (auto func: callback) func(token);
}

void TokenParser::SetStartCallback(HandlerFunc f) {
    start.push_back(f);
}

void TokenParser::SetAnyTokenCallback(HandlerFunc f) {
    anytok.push_back(f);
}

void TokenParser::SetStringTokenCallback(HandlerFunc f) {
    strtok.push_back(f);
}

void TokenParser::SetDigitTokenCallback(HandlerFunc f) {
    digtok.push_back(f);
}

void TokenParser::SetEndCallback(HandlerFunc f) {
    end.push_back(f);
}