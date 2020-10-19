#pragma once
#include <string>
#include <functional>
#include <vector>

using HandlerFunc = std::function <void (const std::string &)>;

class TokenParser {
    public:
        TokenParser(){}
        void SetStartCallback(HandlerFunc f);
        void SetDigitTokenCallback(HandlerFunc f);
        void SetStringTokenCallback(HandlerFunc f);
        void SetAnyTokenCallback(HandlerFunc f);
        void SetEndCallback(HandlerFunc f);
        void Parse(const std::string & text);
        void RunHandlers(const std::vector<HandlerFunc> &callback,
                         const std::string &token);

    private:
    std::vector<HandlerFunc> start = {};
    std::vector<HandlerFunc> end = {};
    std::vector<HandlerFunc> anytok = {};
    std::vector<HandlerFunc> strtok = {};
    std::vector<HandlerFunc> digtok = {};
};
