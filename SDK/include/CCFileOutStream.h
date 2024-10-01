#ifndef CCFileOutStream_H
#define CCFileOutStream_H
#include "CC.h"
#include <fstream>
#include "CCString.h"
#include "vector"
#include "mutex"

using CCOutStream = std::ofstream;
class CCFileOutStream:public CCOutStream
{
private:
    inline static std::mutex M_mutexs;
public:
    using std::ofstream::basic_ofstream;
    CCFileOutStream() = default;
    CCFileOutStream(CCString Path,openmode mode);
    static void WriteFile(const CCString& filename, const CCString& content,openmode mode = std::ios::out | std::ios::binary);
    static void Append(const CCString& filename, const std::vector<char>& data,openmode mode = std::ios::app | std::ios::binary);
    static void Append(const CCString& filename, CCString data,openmode mode = std::ios::app | std::ios::binary);



private:

};


#endif
