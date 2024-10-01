#ifndef CCBYTE_CCBYTEARRAY_H
#define CCBYTE_CCBYTEARRAY_H

#include <vector>
#include "CCByte.h"
#include "CCEncode.h"
class CCByteArray
{
private:
    std::vector<CCByte> bytes;

public:
    // 构造函数
    CCByteArray() {}
    // 添加一个字节
    void add(CCByte byte);
    // 获取指定索引处的字节
    CCByte get(int index) const;
    // 设置指定索引处的字节
    void set(int index, CCByte byte);
    // 删除指定索引处的字节
    void remove(int index);
    // 获取字节数组的大小
    size_t size() const;
    // 清空字节数组
    void clear();
    // 将 std::string 转换为 ByteArray
    static CCByteArray fromString(const std::string& str);
    // 将 ByteArray 转换为 std::string
    std::string toString() const;
    std::string Format(const std::string& EncodeStr = "GBK") const;
    std::vector<CCByte> data();
    void resize(int len);
};


#endif
