#include "CCFIleInStream.h"

CCString CCFileInStream::ReadFileDataAll(const CCString &filename) {
    CCInStream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return nullptr;
    }
    CCString fileContent((std::istreambuf_iterator<char>(inputFile)),
                            std::istreambuf_iterator<char>());
    inputFile.close();
    return fileContent;
}

bool CCFileInStream::getline(CCFileInStream &file, CCString &line) {
    bool ret = std::getline(file, line) ? true : false;
    return ret;
}

bool CCFileInStream::getline(CCString &line) {
    CCVar A = std::getline(*this, line) ? true : false;
    return A;
}
