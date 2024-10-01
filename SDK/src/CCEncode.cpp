#include "CCEncode.h"

std::string CCEncode::Format(const std::string& utf8Str, const std::string &Estr) {
    auto A = Format(utf8Str,"UTF-8",Estr);
    return A;
}

std::string CCEncode::Format(const std::string &str, const std::string from, const std::string to) {
    std::vector<char> data(str.begin(), str.end());
    auto A = reiconv::encode(from, to, data);
    return std::string(A.begin(), A.end());
}
