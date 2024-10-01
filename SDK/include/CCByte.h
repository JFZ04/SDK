#ifndef CCCCByte_CCCCByte_H
#define CCCCByte_CCCCByte_H

#include <iostream>
#include <string>

class CCByte {
private:
    unsigned char value;
public:
    CCByte& operator=(const CCByte& other);
    CCByte(char& val);
    // 构造函数
    explicit CCByte(unsigned char val = 0) : value(val) {}
    // 获取当前值
    unsigned char getValue() const;
    // 设置当前值
    void setValue(unsigned char val);
    // 获取指定位的状态 (0 或 1)
    bool getBit(int pos) const;
    // 设置指定位的状态
    void setBit(int pos, bool on);
    // 翻转指定位的状态
    void flipBit(int pos);
    // 字节之间的按位与操作
    CCByte operator&(const CCByte& other) const;
    // 字节之间的按位或操作
    CCByte operator|(const CCByte& other) const;
    // 字节之间的按位异或操作
    CCByte operator^(const CCByte& other) const;
    // 字节之间的按位取反操作
    CCByte operator~() const;
    // 将字节转换为本地编码的 8 位字符
};



#endif
