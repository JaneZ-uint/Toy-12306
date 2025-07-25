//
// Created by JaneZ on 2025/5/17.
//

#ifndef MEMORYRIVER_H
#define MEMORYRIVER_H
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
    friend class Finance;
    friend class Blog;

private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {
    }

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::in | std::ios::out);
        if (!file) {
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i) {
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            }
        }
        file.close();
    }

    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::fstream::in | std::fstream::binary);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name, std::fstream::in | std::fstream::out | std::fstream::binary);
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void read(T &t, const int index) {
        file.open(file_name, std::fstream::in | std::fstream::binary);
        file.seekg(static_cast<long long int>(index) * sizeofT + info_len * sizeof(int));
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void write(T &t, const int index) {
        file.open(file_name, std::fstream::in | std::fstream::out | std::fstream::binary);
        file.seekp(static_cast<long long int>(index) * sizeofT + info_len * sizeof(int));
        file.write(reinterpret_cast<const char *>(&t), sizeofT);
        file.close();
    }

    void clear() {
        file.open(file_name);
        file.clear();
        file.close();
    }
};


#endif //MEMORYRIVER_H
