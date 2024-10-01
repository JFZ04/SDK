#ifndef THREADPOOLDEMO_CCJSONOBJECT_H
#define THREADPOOLDEMO_CCJSONOBJECT_H
#include "nlohmann/json.hpp"
#include "CC.h"
using JSON = nlohmann::json;

class CCJSONObject: public JSON
{
public:
    using nlohmann::json::json;
    CCJSONObject() = default;
    CCJSONObject(JSON && j);
    void put(const char* key,nlohmann::json value);
    JSON get(const char* key);
    std::string to_String();
    static CCJSONObject parse(const String& str);
private:
    JSON J;
};


#endif
