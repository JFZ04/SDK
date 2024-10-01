#include "CCFileOutStream.h"

void CCFileOutStream::WriteFile(const CCString &filename, const CCString &content,openmode mode) {
    // 使用ofstream对象打开或创建文件
    std::ofstream file(filename,mode);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    // 写入内容到文件
    file << content;
    // 关闭文件
    file.close();
}

void CCFileOutStream::Append(const CCString &filename, const std::vector<char>& data,openmode mode) {
    // 使用ofstream对象打开或创建文件，并设置为追加模式
    M_mutexs.lock();
    std::ofstream file(filename, mode);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        M_mutexs.unlock();
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    // 写入数据到文件
    file.write(data.data(), data.size());
    // 关闭文件
    file.close();
    M_mutexs.unlock();
}

void CCFileOutStream::Append(const CCString &filename, CCString data, std::ios_base::openmode mode) {
// 使用ofstream对象打开或创建文件，并设置为追加模式
    std::ofstream file(filename, mode);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }
    // 写入数据到文件
    file.write(data.data(), data.size());
    // 关闭文件
    file.close();
}

CCFileOutStream::CCFileOutStream(CCString Path, std::ios_base::openmode mode) {
    open(Path,mode);
}
