#include "TokenParser.h"
#include <iostream>
#include <stdexcept>

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name) {
    try {
      func();
      std::cerr << test_name << " OK" << std::endl;
    } catch (std::exception& e) {
      ++fail_count;
      std::cerr << test_name << " fail: " << e.what() << std::endl;
    } catch (...) {
      ++fail_count;
      std::cerr << "Unknown exception caught" << std::endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

using strvec = std::vector<std::string>;

int start = 0, end = 0, tokens_count = 0;
strvec tokens, nums, strings;

void check_start(const std::string &) {
    start++;
}

void check_end(const std::string &) {
    end++;
}

void check_tokens(const std::string &tok) {
    tokens.push_back(tok);
}

void check_nums(const std::string &num) {
    nums.push_back(num);
}

void check_strings(const std::string &string) {
    strings.push_back(string);
}

void ckeck_tokens_count(const std::string &) {
    tokens_count++;
}

void Test_1() {
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);

    parser.Parse("One 2 thre-e f0ur _five $1x");

    if (start != 1)
        throw std::runtime_error("Start Callback!");
    if (end != 1)
        throw std::runtime_error("End Callback!");
    if (tokens.size() != 6)
        throw std::runtime_error("Parsing error!");
    if (nums.size() != 1) 
        throw std::runtime_error("Nums parsing error!");
    if (strings.size() != 5) 
        throw std::runtime_error("Strings parsing error!");
    
}

void Test_2() { // пустой текст 
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);

    parser.Parse("");
    if (start != 1) 
        throw std::runtime_error("Start Callback!");
    if (end != 1) 
        throw std::runtime_error("End Callback!");
    if (tokens.size() != 0) 
        throw std::runtime_error("Parsing error!");
    if (nums.size() != 0) 
        throw std::runtime_error("Nums parsing error!");
    if (strings.size() != 0) 
        throw std::runtime_error("Strings parsing error!");
}

void Test_3() { // несколько обработчиков
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetAnyTokenCallback(ckeck_tokens_count); // 
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);

    parser.Parse("Mo, 19 october 03:07     .");
    if (start != 1) 
        throw std::runtime_error("Start Callback!");
    if (end != 1) 
        throw std::runtime_error("End Callback!");
    if (tokens.size() != 5)
        throw std::runtime_error("Parsing error!");
    if (tokens_count != 5)
        throw std::runtime_error("Parsing error!");
    if (nums.size() != 1) 
        throw std::runtime_error("Nums parsing error!");
    if (strings.size() != 4) 
        throw std::runtime_error("Strings parsing error!");
}

int main() {
    TestRunner runner;

    runner.RunTest(Test_1, "Test_1");
    start = 0, end = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_2, "Test_2");
    start = 0, end = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_3, "Test_3");
    start = 0, end = 0, tokens_count = 0;
    tokens.clear(), nums.clear(), strings.clear();

    return 0;
}