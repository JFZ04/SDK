#include "../include/CCByte.h"

unsigned char CCByte::getValue() const {
    return value;
}

void CCByte::setValue(unsigned char val) {
    value = val;
}

bool CCByte::getBit(int pos) const {
    if (pos < 0 || pos > 7) return false; // 检查位置是否有效
    return (value & (1 << pos)) != 0;
}

void CCByte::setBit(int pos, bool on) {
    if (pos < 0 || pos > 7) return; // 检查位置是否有效
    if (on) {
        value |= (1 << pos);
    } else {
        value &= ~(1 << pos);
    }
}

void CCByte::flipBit(int pos) {
    if (pos < 0 || pos > 7) return; // 检查位置是否有效
    value ^= (1 << pos);
}

CCByte CCByte::operator&(const CCByte &other) const {
    return CCByte(value & other.value);
}

CCByte CCByte::operator|(const CCByte &other) const {
    return CCByte(value | other.value);
}

CCByte CCByte::operator^(const CCByte &other) const {
    return CCByte(value ^ other.value);
}

CCByte CCByte::operator~() const {
    return CCByte(~value);
}

CCByte::CCByte(char& val) {
    value = static_cast<unsigned char>(val);
}

CCByte &CCByte::operator=(const CCByte &other) = default;
