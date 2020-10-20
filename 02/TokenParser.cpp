#include "TokenParser.h"
#include <sstream>
#include <cctype>
#include <cstring>

void TokenParser::Parse(const std::string & text) {
    std::string token;
    std::stringstream ss(text);
    RunHandlers(start, text);
    while (ss >> token) {
        std::string cut;
        bool is_num = true;
        for (auto c: token) {
            switch (c) {
            case '{': 
                ++bracket_count;
                if (cut != "") {
                    RunHandlers(anytok, cut);
                    if (is_num) 
                        RunHandlers(digtok, cut);
                    else 
                        RunHandlers(strtok, cut);
                    cut = "", is_num = true;
                }
                RunHandlers(left_br, "{");
                break;
            case '}':
                --bracket_count;
                if (bracket_count < 0) throw std::runtime_error("Not valid!");
                if (cut != "") {
                    RunHandlers(anytok, cut);
                    if (is_num) 
                        RunHandlers(digtok, cut);
                    else 
                        RunHandlers(strtok, cut);
                    cut = "", is_num = true;
                }
                RunHandlers(right_br, "}");
                break;
            default:
                if(!isdigit(c)) is_num = false;
                cut += c;
            }
        }
        if (cut != "") {
            RunHandlers(anytok, cut);
            if (is_num) 
                RunHandlers(digtok, cut);
            else 
                RunHandlers(strtok, cut);
            cut = "", is_num = true;
        }
    }
    if (bracket_count != 0) throw std::runtime_error("Not equal!");
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
void TokenParser::SetLeftBrCallback(HandlerFunc f) {
    left_br.push_back(f);
}

void TokenParser::SetRightBrCallback(HandlerFunc f) {
    right_br.push_back(f);
}

void TokenParser::SetEndCallback(HandlerFunc f) {
    end.push_back(f);
}