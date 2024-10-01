#include "CCByteArray.h"

void CCByteArray::add(CCByte byte) {
    bytes.push_back(byte);
}

CCByte CCByteArray::get(int index) const {
    if (index < 0 || index >= bytes.size()) {
        throw std::out_of_range("Index out of range");
    }
    return bytes[index];
}

void CCByteArray::set(int index, CCByte byte) {
    if (index < 0 || index >= bytes.size()) {
        throw std::out_of_range("Index out of range");
    }
    bytes[index] = byte;
}

void CCByteArray::remove(int index) {
    if (index < 0 || index >= bytes.size()) {
        throw std::out_of_range("Index out of range");
    }
    bytes.erase(bytes.begin() + index);
}

size_t CCByteArray::size() const {
    return bytes.size();
}

void CCByteArray::clear() {
    bytes.clear();
}

CCByteArray CCByteArray::fromString(const std::string &str) {
    CCByteArray byteArray;
    for (char c : str) {
        byteArray.add(CCByte(static_cast<unsigned char>(c)));
    }
    return byteArray;
}

std::string CCByteArray::toString() const {
    std::string result;
    for (const CCByte& byte : bytes) {
        result += static_cast<char>(byte.getValue());
    }
    return result;
}

std::string CCByteArray::Format(const std::string &EncodeStr) const {
    return CCEncode::Format(this->toString(), EncodeStr);
}

std::vector<CCByte> CCByteArray::data() {
    return bytes;
}

void CCByteArray::resize(int len) {
    bytes.resize(len);
}
