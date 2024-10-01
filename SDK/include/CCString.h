#ifndef STM32API_CCSTRING_H
#define STM32API_CCSTRING_H
#include "CCVector.h"
#include <cstdio>

#define PC
#ifdef PC
#include <cstring>
#include "strstream"
#include "sstream"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>
#include "CCEncode.h"
#include "CCByteArray.h"

using CCStream = std::strstream;
class CCString : public std::string
{
public:
    using std::string::string;
    CCString(basic_string<char> basicString);
    CCString Format(const CCString& EncodeStr);
    CCByteArray getByteArray(const CCString& EncodeStr = "UTF-8");
    int to_int();
    float to_float();
    double to_double();
    template<typename... T>
    static CCString format(int Size,const char *str,T&&... arg) {
        char data[Size] = {0};
        sprintf(data,str,arg...);
        return data;
    }
};
#else
class CCString
{
public:
    CCString() = default;
    CCString(const char* str);
    CCString(const CCString& str);
    CCString(CCVector<char> str);

    //----------------------------------------------------------------
    const char* c_str();
    unsigned int length();
    float to_float();
    int to_int();
    void append(char str);
    void append(const char* str);
    void append(CCString str);
    void append(CCVector<char> str);
    void clear();
    //----------------------------------------------------------------
    bool operator==(const char* str);
    bool operator==(const CCString str);
    bool operator!=(const char* str);
    bool operator!=(const CCString& str);
    //----------------------------------------------------------------
    CCString operator+(const char* str);
    void erase(int index,int len);
    unsigned int find(char str);
    unsigned int find(const char* str);
    unsigned int find(CCString str);
    void replace(int index,char str);
    void replace(int index,const char* str);
    void replace(int index,int index2, const char *str);
    //----------------------------------------------------------------
#ifdef __JSON__
    JSONObject to_json()
    {
        return JSONObject::Parse(c_str());
    }
#endif

private:
    CCVector<char> data;
    void copy(const char* str);
    void copy(char* str,const char* str2,int len);
    void copy(char* str,CCVector<char> str2,int len);
    int strlenth(const char* str);

};

namespace CCTools
{
    inline CCString To_CCString(int index)
    {
        char str[100] = {0};
        sprintf(str,"%d",index);
        return str;
    }

    inline CCString To_CCString(float index)
    {
        char str[100] = {0};
        sprintf(str,"%.5f",index);
        return str;
    }

    inline CCString To_CCString(double index)
    {
        char str[100] = {0};
        sprintf(str,"%lf",index);
        return str;
    }
    template<typename T>
    inline CCString To_CCString(const char* l,T num)
    {
        char str[100] = {0};
        sprintf(str,l,num);
        return str;
    }

#ifdef __JSON__
    inline CCString To_CCString(JSONObject a)
    {
        return "";
    }
#endif
    enum Error_type
    {
        Lengtherror = -1,
    };
}
#endif

#endif
