#ifndef CC_Web_CORS_H
#define CC_Web_CORS_H


#include <vector>
#include "CCString.h"
#include "iostream"

class CORS
{
public:
    CORS() = default;
    void AddAllowOrigin(const CCString& Origin);
    void AddHeader(const CCString& Key,const CCString& Value);
    std::vector<CCString> GetAllowOrigin();
    std::map<CCString,CCString> GetHeader();

private:
    std::map<CCString,CCString> CORSA;
    std::vector<CCString> AllowOrigin;

};

inline void CORS::AddAllowOrigin(const CCString& Origin) {
    AllowOrigin.push_back(Origin);
}

inline void CORS::AddHeader(const CCString &Key, const CCString &Value) {
    CORSA.insert(std::pair<CCString,CCString>(Key,Value));
}

inline std::vector<CCString> CORS::GetAllowOrigin() {
    return AllowOrigin;
}

inline std::map<CCString,CCString> CORS::GetHeader() {
    return CORSA;
}


#endif
