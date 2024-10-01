#ifndef T_CC_H
#define T_CC_H

#include <sstream>
#include <iomanip>
#include "iostream"
#include "mutex"

#define CCVar auto
using String = std::string;
typedef std::exception CCException;
typedef std::unique_lock<std::mutex> CCUniqueLock;

namespace CC
{
    inline String to_String(int v){
        return std::to_string(v);
    }
    inline String to_String(double v){
        return std::to_string(v);
    }
    inline String to_String(bool v){
        return v ? "true" : "false";
    }
    inline String to_String(const char* v){
        return v;
    }
    inline String to_String(String v){
        return v;
    }
    inline String to_String(void* v){
        return std::to_string((size_t)v);
    }
    template<typename T>
    inline void Print(const T& v){
        std::cout << v;
    }
    template<typename T,typename... Args>
    inline void Print(const T& v,Args&&... args){
        std::cout << v;
        Print(args...);
    }
    template<typename T>
    inline void Println(const T& v){
        std::cout<< v << std::endl;
    }
}
class CCInt
{
private:
    int value;
public:
    CCInt() = default;
    CCInt(int v){
        this->value = v;
    }
    String to_String() const{
       return std::to_string(value);
    }
};

class CCDouble
{
private:
    double value;
public:
    CCDouble() = default;
    CCDouble(double v){
        this->value = v;
    }
    String to_String(int x = 5) const{
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(x) << value;
        return stream.str();
    }
};

class CCFloat
{
private:
    float value;
public:
    CCFloat() = default;
    CCFloat(float v){
        this->value = v;
    }
    String to_String(int x = 5) const{
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(x) << value;
        return stream.str();
    }
};

#endif
