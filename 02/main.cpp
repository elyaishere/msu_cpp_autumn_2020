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

int start = 0, end = 0, tokens_count = 0, left_br = 0, right_br = 0;
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

void check_tokens_count(const std::string &) {
    tokens_count++;
}

void check_left_br(const std::string &) {
    left_br++;
}

void check_right_br(const std::string &) {
    right_br++;
}

void Test_1() {
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {

        parser.Parse("One 2 thre-e f0ur _five $1x");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

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
    if (left_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
    if (right_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
    
}

void Test_2() { // пустой текст 
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

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
    if (left_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
    if (right_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
}

void Test_3() { // несколько обработчиков
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetAnyTokenCallback(check_tokens_count); // 
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("Mo, 19 october 03:07     .");
    }  catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

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
    if (left_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
    if (right_br != 0) 
        throw std::runtime_error("Brackets parsing error!");
}

void Test_4_new() {   
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("{test} { 12 test2\n}\t");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    if (start != 1) 
        throw std::runtime_error("Start Callback!");
    if (end != 1) 
        throw std::runtime_error("End Callback!");
    if (tokens.size() != 3) 
        throw std::runtime_error("Parsing error!");
    if (nums.size() != 1) 
        throw std::runtime_error("Nums parsing error!");
    if (strings.size() != 2) 
        throw std::runtime_error("Strings parsing error!");
    if (left_br != 2) 
        throw std::runtime_error("Brackets parsing error!");
    if (right_br != 2) 
        throw std::runtime_error("Brackets parsing error!");
}

void Test_5_new() {   
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("{test{ 123 }");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    throw std::runtime_error("Wrong parsing!");
}

void Test_6_new() {   
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("}12{");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    throw std::runtime_error("Wrong parsing!");
}

void Test_7_new() {   
    TokenParser parser;
    parser.SetStartCallback(check_start);
    parser.SetAnyTokenCallback(check_tokens);
    parser.SetDigitTokenCallback(check_nums);
    parser.SetStringTokenCallback(check_strings);
    parser.SetEndCallback(check_end);
    parser.SetLeftBrCallback(check_left_br);
    parser.SetRightBrCallback(check_right_br);

    try {
        parser.Parse("1234{acd}4321 {}{\tavc{}}");
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    if (start != 1) 
        throw std::runtime_error("Start Callback!");
    if (end != 1) 
        throw std::runtime_error("End Callback!");
    if (tokens.size() != 4) 
        throw std::runtime_error("Parsing error!");
    if (nums.size() != 2) 
        throw std::runtime_error("Nums parsing error!");
    if (strings.size() != 2) 
        throw std::runtime_error("Strings parsing error!");
    if (left_br != 4) 
        throw std::runtime_error("Brackets parsing error!");
    if (right_br != 4) 
        throw std::runtime_error("Brackets parsing error!");
}

int main() {
    TestRunner runner;

    runner.RunTest(Test_1, "Test_1");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_2, "Test_2");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_3, "Test_3");
    start = 0, end = 0, tokens_count = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_4_new, "Test_4_new");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_5_new, "Test_5");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_6_new, "Test_6_new");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    runner.RunTest(Test_4_new, "Test_7_new");
    start = 0, end = 0, left_br = 0, right_br = 0;
    tokens.clear(), nums.clear(), strings.clear();

    return 0;
}