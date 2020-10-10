#include "Allocator.h"
#include <iostream>
#include <stdexcept>
#include <string>

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

void Test_alloc() {
    Allocator a1(100);

    char * ptr = a1.alloc(40);
    if (ptr == nullptr) {
        throw runtime_error("Allocation failed");
    }

    ptr = a1.alloc(50);
    if (ptr == nullptr) {
        throw runtime_error("Allocation failed");
    }

    ptr = a1.alloc(20);
    if (ptr != nullptr) {
        throw runtime_error("Allocation failed");
    }

    a1.reset();
    ptr = a1.alloc(100);
    if (ptr == nullptr) {
        throw runtime_error("Reset failed");
    }
}


void Test_Zero_alloc () {
    Allocator a2(0);

    char * ptr = a2.alloc(0);
    if (ptr == nullptr) {
        throw runtime_error("Allocation failed");
    }

}

void Test_Realloc () {
    Allocator a3(10);

    a3.makeAllocator(30);
    char * ptr = a3.alloc(20);
    if (ptr == nullptr) {
        throw runtime_error("Allocation failed");
    }
}

int main() {
    TestRunner runner;

    runner.RunTest(Test_alloc, "Test_alloc");
    runner.RunTest(Test_Zero_alloc, "Test_Zero_alloc");
    runner.RunTest(Test_Realloc, "Test_Realloc");

    return 0;
}