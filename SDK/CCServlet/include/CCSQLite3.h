#ifndef THREADPOOLDEMO_CCSQLITE3_H
#define THREADPOOLDEMO_CCSQLITE3_H
#pragma once
#include "sqlite3.h"
#include "CCString.h"
#include "vector"
#include "mutex"
#include "CCJSONObject.h"

/*
* CREATE TABLE database_name.table_name(
           column1 datatype  PRIMARY KEY(one or more columns),
           column2 datatype,
           column3 datatype,
           .....
           columnN datatype,);

 *  DROP TABLE database_name.table_name;
 *  INSERT INTO TABLE_NAME VALUES (value1,value2,value3,...valueN);
 *  SELECT * FROM table_name;
 *  SELECT * FROM table_name WHERE xx='xx';
 *  UPDATE xxx SET ADDRESS = 'Texas' WHERE xxx = xxx;
 *  DELETE FROM table_name WHERE xxx=xxx;
 *
 * */
struct CCResultSet
{
    std::vector<JSON> Data;
    int Row = 0;
    int Col = 0;
    bool Flag = false;
};

class CCSQLite3
{
public:
    CCSQLite3(CCString Path);
    bool Open(CCString Path);
    bool Close();
    bool SqlUpdate(CCString sql);
    CCResultSet SqlQuery(CCString sql);
    CCString GetError();

private:
    sqlite3* SQL;
    bool SQlite(const char* sql, int(*callback)(void*, int, char**, char**) = NULL, void* a = NULL);
    char *zErrMsg = nullptr;
    std::mutex mutex;

// 检查字符串中是否包含小数点
    bool containsDecimal(const std::string &str) {
        for (char ch : str) {
            if (ch == '.') {
                return true;
            }
        }
        return false;
    }

// 根据字符串内容转换为 int 或 double
    template<typename T>
    T convert(const std::string &str) {
        std::istringstream iss(str);
        T value;
        iss >> value;
        if (iss.fail()) {
            throw std::invalid_argument("Conversion failed.");
        }
        return value;
    }

};


#endif
