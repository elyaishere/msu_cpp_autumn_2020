#include "BigInt.h"
#include <stdexcept>

namespace std {
class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};
}

void Plus1 () {
    BigInt a1 = 1;
    BigInt a2 = 0;
    BigInt a3 = -1;
    BigInt a4 = -2;
    BigInt a5("1999999999");
    BigInt a6("-20000");

    int error_count = 0;
    if (a1 + 1 != 2) {
        ++error_count;
    }
    if (a2 + 1 != 1) {
        ++error_count;
    }
    if (a3 + 1 != 0) {
        ++error_count;
    }
    if (a4 + 1 != -1) {
        ++error_count;
    }
    if (a5 + 1 != BigInt("2000000000")) {
        ++error_count;
    }
    if (a6 + 1 != BigInt("-19999")) {
        ++error_count;
    }
    if (error_count) {
        throw std::runtime_error("+ 1 failed");
    }
}
void Cmp () {
    int error_count = 0;
    if (!(BigInt(5556) > 5554)){
        ++error_count;
    }
    if (!(BigInt(-999999) < 0)){
        ++error_count;
    }
    if (!(BigInt(123) == 123)){
        ++error_count;
    }
    if (!(BigInt(45) >= 45)){
        ++error_count;
    }
    if (!(BigInt("-3333") <= -3333)){
        ++error_count;
    }
    if (!(BigInt(45) != -45)){
        ++error_count;
    }
    if (error_count) {
        throw std::runtime_error("Compare failed");
    }

}
void Diff() {
    int error_count = 0;
    if (BigInt(1999) - BigInt(999) != 1000) {
        ++error_count;
    }
    if (BigInt(0) - BigInt(10) != -10) {
        ++error_count;
    }
    if (BigInt(-36) - BigInt(4) != -40) {
        ++error_count;
    }
    if (BigInt("1999999999999999") - BigInt("-1") != BigInt("2000000000000000")) {
        ++error_count;
    }
    if (-100 - BigInt(-50) != -50) {
        ++error_count;
    }
    if (BigInt("-50000000000") - BigInt("-50000000001") != 1) {
        ++error_count;
    }
    if (error_count) {
        throw std::runtime_error("Diff failed");
    }
}
void Sum() {
    int error_count = 0;
    if (BigInt(1999) + BigInt(91) != 2090) {
        ++error_count;
    }
    if (BigInt(0) + BigInt(55) != 55) {
        ++error_count;
    }
    if (BigInt(6) + BigInt(-6) != 0) {
        ++error_count;
    }
    if (BigInt("1999999999999999") + BigInt(1) != BigInt("2000000000000000")) {
        ++error_count;
    }
    if (-100 + BigInt(-50) != -150) {
        ++error_count;
    }
    if (BigInt("50000000000") + BigInt("-50000000001") != -1) {
        ++error_count;
    }
    if (error_count) {
        throw std::runtime_error("Summ failed");
    }
}
void Mul() {
    int error_count = 0;
    if (BigInt(11) * BigInt(11) != 121) {
        ++error_count;
    }
    if (BigInt(0) * BigInt("19999999999999999") != 0) {
        ++error_count;
    }
    if (BigInt(-6) * BigInt(-6) != 36) {
        ++error_count;
    }
    if (BigInt(9999) * BigInt(-999) != BigInt("-9989001")) {
        ++error_count;
    }
    if (-10 * BigInt(25) != -250) {
        ++error_count;
    }
    if (BigInt("12345678901234567890") * 1 != BigInt("12345678901234567890")) {
        ++error_count;
    }
    if (error_count) {
        throw std::runtime_error("Mul failed");
    }    
}

int main() {
    std::TestRunner runner;

    runner.RunTest(Plus1, "Plus1");
    runner.RunTest(Cmp, "Cmp");
    runner.RunTest(Sum, "Sum");
    runner.RunTest(Diff, "Diff");
    runner.RunTest(Mul, "Mul");

    return 0;
}
