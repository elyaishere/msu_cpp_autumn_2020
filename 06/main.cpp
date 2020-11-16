#include <iostream>
#include "format.h"
#include <cassert>

int main()
{
    // test 1
    {
        auto text = format("{1}+{1} = {0}", 2, "one");
        assert(text == "one+one = 2");
    }
    // test 2
    {
        try {
            auto text = format("Missing {} index", '?');
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Wrong value inside {}";
            assert(e.what() == what);
        }
    }

    // test 3
    {
        try {
            auto text = format("Wrong {20} index", 'x');
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Wrong value inside {}";
            assert(e.what() == what);
        }
    }

    // test 4
    {
        try {
            auto text = format("Too much {0} {1} indices", 0);
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Wrong value inside {}";
            assert(e.what() == what);
        }
    }

    // test 5
    {
        try {
            auto text = format("Too much { {} {0} {1}", 0);
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Double {";
            assert(e.what() == what);
        }
    }

    // test 6
    {
        try {
            auto text = format("Too much {0} } {1}", 0);
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Unexpected }";
            assert(e.what() == what);
        }
    }

    // test 7
    {
        try {
            auto text = format("Not enough {0} {", 0);
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Expected }";
            assert(e.what() == what);
        }
    }

    // test 8
    {
        auto text = format("It's {0} now", "02:00");
        assert(text == "It's 02:00 now");
    }

    // test 9
    {
        auto text = format("No format");
        assert(text == "No format");
    }

    // test 10
    {
        try {
            auto text = format("No format but wrong {}");
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Unexpected bracket";
            assert(e.what() == what);
        }
    }

    // test 11
    {
        try {
            auto text = format("Is {it} an index?", "no");
            assert(0 == 1);
        } catch (const std::exception & e) {
            std::string what = "Wrong value inside {}";
            assert(e.what() == what);
        }
    }
    
    std::cout << "All tests passed" << std::endl;

    return 0;
}