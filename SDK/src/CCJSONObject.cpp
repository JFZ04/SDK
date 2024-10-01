#include "../include/CCJSONObject.h"

void CCJSONObject::put(const char *key, nlohmann::json value) {
    J[key] = value;
}

JSON CCJSONObject::get(const char *key) {
   return J[key];
}

std::string CCJSONObject::to_String() {
    return J.dump();
}

CCJSONObject::CCJSONObject(JSON &&j) {
    J = j;
}

CCJSONObject CCJSONObject::parse(const String &str) {
    try {
        return nlohmann::json::parse(str);
    }
    catch (...) {
        return nullptr;
    }
}

