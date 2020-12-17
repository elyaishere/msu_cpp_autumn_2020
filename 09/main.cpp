#include <iostream>
#include <fstream>
#include <thread>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#define SIZE 1024 * 1024 // 8Mbytes / sizeof(uint64_t)

using namespace std;

uint64_t buffer[SIZE];
vector<string> files;

void fSort(string fname) {
    ifstream input(fname, ios::binary);
    size_t chunks = 0;
    input.seekg (0, input.end);
    size_t l = input.tellg();
    input.seekg (0, input.beg);
    if (l == 0) {
        files.emplace_back("tmp_1");
        ofstream t(files[0], ios::binary);
        t.close();
        input.close();
        return;
    }
    
    while (l > 0) {
        //cout << "l: " << l << endl;
        
            input.read(reinterpret_cast<char *>(buffer), SIZE*sizeof(uint64_t));
            auto sz = input.gcount();
            l -= sz;
            sz /= sizeof(uint64_t);
            if (sz > SIZE / 2)
            {
                for (size_t i = 0; i < 2; ++i) {
                stringstream ss;
                ss << "src_"  + to_string(chunks++);
                files.emplace_back(ss.str());
                ss.clear();
                }
                thread t1([]() { sort(buffer, buffer + (SIZE / 2));});
                thread t2([sz]() { sort(buffer + (SIZE / 2), buffer + sz); });
                t1.join();
                t2.join();
            
                ofstream out1(files[chunks - 1], ios::binary);
                ofstream out2(files[chunks - 2], ios::binary);
                out1.write(reinterpret_cast<char *>(buffer), sizeof(uint64_t) * (SIZE / 2));
                out2.write(reinterpret_cast<char *>(buffer + SIZE / 2), sizeof(uint64_t) * (sz - SIZE / 2));
                out2.close();
                out1.close();
            } 
            else 
            {
                stringstream ss;
                ss << "src_"  + to_string(chunks++);
                files.emplace_back(ss.str());
                ss.clear();
                sort(buffer, buffer + sz);
                ofstream out1(files[chunks - 1], ios::binary);
                out1.write(reinterpret_cast<char *>(buffer), sizeof(uint64_t) * (sz));
                out1.close();
            }
    }
    input.close();
    for (size_t i = 0; i < chunks - 1; ++ i) {
        ifstream src1(files[i + 1], ios::binary); // следующий
        src1.read(reinterpret_cast<char *>(buffer), sizeof(uint64_t)*SIZE);
        auto sz = src1.gcount();
        src1.close();
        ifstream src2(files[i], ios::binary);
        files[i + 1] = "tmp_" + to_string(i + 1);
        ofstream res(files[i + 1], ios::binary);
        uint64_t cur_buf = 0;
        uint64_t cur_src;
        src2.read(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
        while(!src2.eof()) {
            if (cur_buf == sz) {
                res.write(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                src2.read(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                continue;
            }
            if (cur_src < buffer[cur_buf]) {
                res.write(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                src2.read(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                continue;
            }
            if (cur_src == buffer[cur_buf]) {
                res.write(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                res.write(reinterpret_cast<char *>(buffer + cur_buf), sizeof(uint64_t));
                src2.read(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
                ++cur_buf;
                continue;
            }
            res.write(reinterpret_cast<char *>(buffer + cur_buf), sizeof(uint64_t));
            ++cur_buf;
        }
        while (cur_buf < sz) { // дописать буффер
            res.write(reinterpret_cast<char *>(buffer + cur_buf), sizeof(uint64_t));
            ++cur_buf;
        }
        src2.close();
        res.close();
    }
    
    
}

void tSort(string fname) {
    ifstream input(fname, ios::binary);
    input.seekg (0, input.end);
    size_t l = input.tellg();
    if (l == 0) {
        ofstream output("check", ios::binary);
        output.close();
        input.close();
        return;
    }
    auto sz = l / sizeof(uint64_t);
    auto *buf = new uint64_t[sz];
    input.seekg (0, input.beg);
    input.read(reinterpret_cast<char *>(buf), l);
    input.close();
    sort(buf, buf + sz);
    ofstream output("check", ios::binary);
    output.write(reinterpret_cast<char *>(buf), l);
    output.close();
}

bool check() {
    ifstream f1("check", ios::binary);
    ifstream f2(files[files.size() - 1], ios::binary);
    f1.seekg (0, f1.end);
    size_t l1 = f1.tellg();
    f2.seekg (0, f2.end);
    size_t l2 = f2.tellg();
    if (l1 != l2) {
        f1.close();
        f2.close();
        cout << l1 << " " << l2 << endl;
        return false;
    }
    if (l1 == 0) {
        f1.close();
        f2.close();
        return true;
    }
    f1.seekg (0, f1.beg);
    f2.seekg (0, f2.beg);
    uint64_t a, b;
    while (!f1.eof()) {
        f1.read(reinterpret_cast<char *>(&a), sizeof(uint64_t));
        f2.read(reinterpret_cast<char *>(&b), sizeof(uint64_t));
        if (a != b) {
            f1.close();
            f2.close();
            return false;
        }
    }
    f1.close();
    f2.close();
    return true;
}

int main() {
    fSort("seq");
    tSort("seq");
    assert(check());
    cout << "Test passed" << endl;
    return 0;
}
