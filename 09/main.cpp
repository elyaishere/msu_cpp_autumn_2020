#include <iostream>
#include <fstream>
#include <thread>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#define SIZE 1024 * 1024 // 8Mbytes / sizeof(uint64_t)

using namespace std;

uint64_t buffer[SIZE];

int main() {
    ifstream input("seq", ios::binary);
    size_t chunks = 0;
    vector<string> files;
    size_t p = 0;
    while (!input.eof()) {
        input.read(reinterpret_cast<char *>(buffer + p), sizeof(uint64_t));
        ++p;
        if (p == SIZE) {
            //cout << p << endl;
            for (size_t i = 0; i < 2; ++i) {
                stringstream ss;
                ss << "src_"  + to_string(chunks ++);
                files.emplace_back(ss.str());
                ss.clear();
            }
            thread t1([files, chunks]() { 
                sort(buffer, buffer + (SIZE / 2));
                });
            thread t2([files, chunks]() { 
                sort(buffer + (SIZE / 2), buffer + SIZE); 
                });
            t1.join();
            t2.join();
            ofstream out1(files[chunks - 1], ios::binary);
            ofstream out2(files[chunks - 2], ios::binary);
            for (size_t i = 0; i < SIZE / 2; ++i) {
                out1.write(reinterpret_cast<char *>(buffer + i), sizeof(uint64_t));
                out2.write(reinterpret_cast<char *>(buffer + SIZE/2 + i), sizeof(uint64_t));
            }
            out2.close();
            out1.close();
            p = 0;
        }
    }
    if (p > 0) {
        stringstream ss;
        ss << "src_" + to_string(chunks ++);
        files.emplace_back(ss.str());
        ss.clear();
        thread t1([p]() { sort(buffer, buffer + p); });
        t1.join();
        ofstream out(files[chunks - 1], ios::binary);
        for (size_t i = 0; i < p; ++i) {
                out.write(reinterpret_cast<char *>(buffer + i), sizeof(uint64_t));
            }
        out.close();
        p = 0;
    }
    input.close();
    if (chunks == 1) {
        ifstream src(files[0], ios::binary);
        ofstream res("tmp_1");
        while (!src.eof())
        {
            src.read(reinterpret_cast<char *>(buffer), sizeof(uint64_t));
            res.write(reinterpret_cast<char *>(buffer), sizeof(uint64_t));
        }
        src.close();
        res.close();
    } else
    {
    for (size_t i = 0; i < chunks - 1; ++ i) {
        ifstream src1(files[i + 1], ios::binary); // следующий
        while(!src1.eof()) {
            src1.read(reinterpret_cast<char *>(buffer + p), sizeof(uint64_t));
            ++p;
        }
        src1.close();
        ifstream src2(files[i], ios::binary);
        files[i + 1] = "tmp_" + to_string(i + 1);
        ofstream res(files[i + 1], ios::binary);
        uint64_t cur_buf = 0;
        uint64_t cur_src;
        src2.read(reinterpret_cast<char *>(&cur_src), sizeof(uint64_t));
        while(!src2.eof()) {
            if (cur_buf == p) {
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
        while (cur_buf < p) { // дописать буффер
            res.write(reinterpret_cast<char *>(buffer + cur_buf), sizeof(uint64_t));
            ++cur_buf;
        }
        src2.close();
        res.close();
        p = 0;
    }
    }
    return 0;
}
