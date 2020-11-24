#include <iostream>
#include "vector.h"
#include <string>
#include <cassert>
#include <sstream>

int main()
{
    // test 1
    {
        vector<int> numbers(4);
        numbers[0] = 0;
        numbers[1] = 2;
        numbers[2] = 4;
        numbers[3] = 6;
        assert(numbers.size() == 4);
        int j = 0;
        for (auto i: numbers)
            assert(i == 2*(j++));
    }
    // test 2
    {
        vector<int> numbers;
        assert(numbers.empty() == true);
        assert(numbers.size() == 0);
        numbers.push_back(1);
        assert(numbers.empty() == false);
        assert(numbers.size() == 1);
        assert(numbers[0] == 1);
    }
    // test 3
    {
        vector<std::string> letters;
        letters.push_back("abc");
        std::string s = "def";
        letters.push_back(s);

        assert(letters.size() == 2);
        assert(letters[0] == "abc");
        assert(letters[1] == "def");
    }
    // test 4
    {  
        vector<int> c(3);
        assert(c.size() == 3);
        c[0] = 1, c[1] = 2, c[2] = 3;
        c.resize(5);
        assert(c.size() == 5);
        c.resize(2);
        assert(c.size() == 2);
        auto r = c.rbegin();
        assert(r != c.rend());
        assert(*r == 2);
        ++r;
        assert(r != c.rend());
        assert(*r == 1);
        ++r;
        assert(r == c.rend());
    }
    // test 5
    {  
        vector<int> d;
        d.emplace_back(1);
        d.push_back(2);
        assert(d.size() == 2);
        d.pop_back();
        assert(d.size() == 1);
        d.clear();
        assert(d.size() == 0);
        assert(d.capacity() != 0);
        try {
            d.pop_back();
        } catch (const std::out_of_range &e) {
            std::string m(e.what());
            assert(m == "Vector is empty");
        } catch(...) {
            assert (1 == 0);
        }
    }
    // test 6
    {  
        vector<int> d(1);
        d[0] = 6;
        d.reserve(15);
        assert(d.capacity() == 15);
        assert(d.size() == 1);
        assert(d[0] == 6);
        d.resize(20);
        assert(d.size() == 20);
        assert(d[0] == 6);
        assert(d.capacity() >= 20);
    }
    // test 7
    {  
        vector<char> d(4);
        d[0] = 'p';
        d[1] = 'a';
        d[2] = 'r';
        d[3] = 't';
        std::string t;
        for (auto i = d.begin(); i != d.end(); ++i)
            t += (*i);
        assert(t == "part");
        t.clear();
        for (auto i = d.rbegin(); i != d.rend(); ++i)
            t += (*i);
        assert(t == "trap");
    }
    // test 8
    {  
        vector<char> v(4);
        v.resize(0);
        assert(v.size() == 0);
        assert(v.begin() == v.end());
    }
    std::cout << "All tests passed" << std::endl;
    return 0;
}