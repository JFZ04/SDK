#include <cassert>
#include "CCString.h"
#define PC
#ifdef PC

#else
void CCString::copy(const char* str)
{
    data.clear();
    for (int i = 0; i < strlenth(str); i++)
    {
        data.push_back(str[i]);
    }
}

int CCString::strlenth(const char* str)
{
    int count = 0;
    while (str[count] != '\0')
    {
        count++;
    }
    return count;
}

CCString::CCString(const char *str)
{
    data.clear();
    for (int i = 0; i < strlenth(str); i++)
    {
        data.push_back(str[i]);
    }
}

CCString::CCString(const CCString &str)
{
    data = str.data;
}

CCString::CCString(CCVector<char> str)
{
    data = str;
}

const char *CCString::c_str()
{
    char* c = (char*)malloc(data.size());
    for (int i = 0; i < data.size();i++)
    {
        c[i] = '\0';
    }
    for (int i = 0; i < data.size(); ++i)
    {
        c[i] = data[i];
    }
    c[data.size()] = '\0';
    return c;
}

unsigned int CCString::length()
{
    return data.size();
}

float CCString::to_float()
{
    char* str = (char*)malloc(data.size());
    copy(str,data,data.size());
    str[data.size()] = '\0';
    char *endptr; // 用于存储解析结束的位置
    double d_value = strtod(str, &endptr); // 将字符串转换为double
    if (endptr == str || *endptr != '\0')
    {
        return 0;
    }
    else
    {
        float f_value = (float)d_value; // 将double转换为float
        return f_value;
    }
}

int CCString::to_int()
{
    char* str = (char*)malloc(data.size());
    copy(str,data,data.size());
    str[data.size()] = '\0';
    char *endptr; // 用于存储解析结束的位置
    double d_value = strtod(str, &endptr); // 将字符串转换为double
    if (endptr == str || *endptr != '\0')
    {
        return 0;
    }
    else
    {
        int f_value = (float)d_value; // 将double转换为float
        return f_value;
    }
}

void CCString::append(const char str)
{
    data.push_back(str);
}

void CCString::clear()
{
    data.clear();
}

void CCString::append(const char *str)
{

    for (int i = 0; i < strlenth(str); i++)
    {
        data.push_back(str[i]);
    }
}

void CCString::append(CCString str)
{
    for (int i = 0; i < str.data.size(); i++)
    {
        data.push_back(str.data[i]);
    }
}

void CCString::append(CCVector<char> str)
{
    for (int i = 0; i < str.size(); i++)
    {
        data.push_back(str[i]);
    }
}

void CCString::copy(char *str, const char *str2, int len)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = str2[i];
    }
}

void CCString::copy(char *str, CCVector<char> str2, int len)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = str2[i];
    }
}

bool CCString::operator==(const char *str)
{
    if(data.size() != strlenth(str))
    {
        return false;
    }
    for (int i = 0; i < strlenth(str); i++)
    {
        if (data[i] != str[i])
        {
            return false;
        }
    }
    return true;
}

bool CCString::operator==(CCString str)
{
    if(data.size() != str.length())
    {
        return false;
    }
    for (int i = 0; i < str.length(); i++)
    {
        if (data[i] != str.data[i])
        {
            return false;
        }
    }
    return true;
}

bool CCString::operator!=(const char *str)
{
    return !this->operator==(str);
}

bool CCString::operator!=(const CCString &str)
{
    return !this->operator==(str);
}

CCString CCString::operator+(const char *str)
{
    CCString newStr(*this);
    newStr.append(str);
    return newStr;
}

void CCString::erase(int index, int len)
{
    data.erase(index, len);
}

unsigned int CCString::find(char str)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] == str)
        {
            return i;
        }
    }
    return -1;
}

unsigned int CCString::find(const char *str)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] == str[0])
        {
            bool flag = true;
            for (int j = 0; j < strlenth(str); j++)
            {
                if (data[i + j] != str[j])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                return i;
            }
        }
    }
    return -1;
}

unsigned int CCString::find(CCString str)
{
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] == str.data[0])
        {
            bool flag = true;
            for (int j = 0; j < str.length(); j++)
            {
                if (data[i + j] != str.data[j])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                return i;
            }
        }
    }
    return -1;
}

void CCString::replace(int index, char str)
{
    data.revise(index,str);
}

void CCString::replace(int index, const char *str)
{
    CCVector<char> newStr;
    for (int i = 0; i < index; i++)
    {
        newStr.push_back(data[i]);
    }
    for (int i = 0; i < strlenth(str); i++)
    {
        newStr.push_back(str[i]);
    }
    for (int i = index + strlenth(str); i < data.size(); i++)
    {
        newStr.push_back(data[i]);
    }
    data = newStr;
}

void CCString::replace(int index,int index2, const char *str)
{
    CCVector<char> newStr;
    for (int i = 0; i < index; i++)
    {
        newStr.push_back(data[i]);
    }
    for (int i = 0; i < strlenth(str); i++)
    {
        newStr.push_back(str[i]);
    }
    for (int i = index2; i < data.size(); i++)
    {
        newStr.push_back(data[i]);
    }
    data = newStr;
}


#endif

CCString::CCString(basic_string<char> basicString) : basic_string(basicString) {

}

CCString CCString::Format(const CCString &EncodeStr) {
    auto A = CCEncode::Format(data(),"UTF-8",EncodeStr);
    return A;
}

CCByteArray CCString::getByteArray(const CCString &EncodeStr) {
    if(EncodeStr == "UTF-8" || EncodeStr == "utf-8"){
        return CCByteArray::fromString(data());
    }
    else{
        return CCByteArray::fromString(CCEncode::Format(data(),"UTF-8",EncodeStr));
    }
}

int CCString::to_int() {
    return atoi(data());
}

float CCString::to_float() {
    return atof(data());
}

double CCString::to_double() {
    return atof(data());
}



