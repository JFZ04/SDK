#ifndef CCFIleInStream_H
#define CCFIleInStream_H
#include "CC.h"
#include "iostream"
#include <fstream>
#include "CCString.h"
#include "vector"
#include "mutex"

using CCInStream = std::ifstream;
class CCFileInStream:public CCInStream
{
public:
    using std::ifstream::basic_ifstream;
private:
    inline static std::mutex M_mutexs;
public:
    static CCString ReadFileDataAll(const CCString& filename);
    static bool getline(CCFileInStream &file,CCString &line);
    bool getline(CCString &line);
private:

protected:

};


#endif
