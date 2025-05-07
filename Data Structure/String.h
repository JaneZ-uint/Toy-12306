//
// Created by JaneZ on 2025/5/5.
//

#ifndef STRING_H
#define STRING_H

#include <cstddef>
#include <ostream>

namespace JaneZ {

    template<size_t string_length>
    class String {
    private:
        char data[string_length + 1] = {'\0'};
    public:
        String() = default;

        String(const char* str) {
            size_t i;
            for(i = 0;i < string_length && str[i] != '\0';i ++) {
                data[i] = str[i];
            }
            data[string_length] = '0';
        }

        char& operator[](size_t index) {
            return data[index];
        }

        const char& operator[](size_t index) const {
            return data[index];
        }

        const char* c_str() const{
            return data;
        }

        constexpr size_t length() const{
            return string_length;
        }

        constexpr size_t capacity() const{
            return string_length + 1;
        }

        bool operator==(const String &other) const {
            return strncmp(data,other.data,string_length) == 0;
        }

        bool operator!=(const String &other) const {
            return !(*this == other);
        }

        bool operator<(const String &other) const {
            return strncmp(data,other.data,string_length) < 0;
        }

        bool operator<=(const String &other) const {
            return strncmp(data,other.data,string_length) <= 0;
        }

        bool operator>(const String &other) const {
            return strncmp(data,other.data,string_length) > 0;
        }

        bool operator>=(const String &other) const {
            return strncmp(data,other.data,string_length) >= 0;
        }

        friend std::ostream& operator <<(std::ostream& os,const String & str) {
            os << str.data;
            return os;
        }
    };
}
#endif //STRING_H
