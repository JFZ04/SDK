#ifndef CC_MySql_H
#define CC_MySql_H

#include "mysql.h"
#include "CCString.h"
#include "CCTimeData.h"
#include "map"
#include "vector"
#include "CCObject.h"

/*
 * create DATABASE xxx;  创建库
 * DROP DATABASE xxx; 删除库
 *
 *
 * //创建表
 * CREATE TABLE xxx (
        column1 datatype,
        column2 datatype,
        ...
    );

    //删除表
    DROP TABLE xxx;  or  DROP TABLE [IF EXISTS] xxx;

    //插入表
    INSERT INTO xxx (column1, column2, column3, ...) VALUES (value1, value2, value3, ...);

    //查询表
    SELECT column1, column2, ... FROM xxx;  or  SELECT * FROM xxx;   or SELECT * FROM xxx WHERE ID='xx';
 * */


struct CCRetData
{
    std::vector<std::vector<CCString>> Data;
};

class CCMySql
{
private:
    MYSQL mysql; //mysql连接
public:
    bool Connect(CCString host,CCString user,CCString passwd,CCString db, int port);
    CCRetData Query(CCString sql);
    bool Execute(CCString sql);
    void Close();
private:

public:

};


#endif
