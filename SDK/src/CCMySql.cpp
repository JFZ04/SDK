#include "CCMySql.h"

bool CCMySql::Connect(CCString host,CCString user,CCString passwd,CCString db, int port) {
    //初始化mysql
    mysql_init(&mysql);  //连接mysql，数据库
//    const char host[] = "localhost";
//    const char user[] = "root";
//    const char psw[] = "111111";
//    const char table[] = "test";
//    const int port = 3306;
    //返回false则连接失败，返回true则连接成功
    if (!(mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0)))
    {
        //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
        printf("Error connecting to database:%s\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Connected...\n");
        return true;
    }
}

CCRetData CCMySql::Query(CCString sql) {
    MYSQL_RES *Res;
    MYSQL_ROW Row;
    CCRetData data;
    if (mysql_query(&mysql, sql.c_str())) {
        CCTimeData time;
        CCString msg = "[" + time.to_String() + "] -> The query failed -> " + mysql_error(&mysql);
        CC::Println(msg);
        return {};
    }
    // 获取结果集
    Res = mysql_store_result(&mysql);
    if (Res == nullptr) {
        CCTimeData time;
        CCString msg = "[" + time.to_String() + "] -> Failed to get the result set -> " + mysql_error(&mysql);
        CC::Println(msg);
        return {};
    }
    // 遍历结果集
    while ((Row = mysql_fetch_row(Res)) != nullptr) {
        CCVar var = mysql_num_fields(Res);
        std::vector<CCString> row;
        for (unsigned int i = 0; i < var; ++i) {
            row.emplace_back(Row[i]);
        }
        data.Data.push_back(row);
    }
    // 释放资源
    mysql_free_result(Res);
    return data;
}

void CCMySql::Close() {
    mysql_close(&mysql);
}

bool CCMySql::Execute(CCString sql) {
    if (mysql_query(&mysql, sql.c_str())) {
        CCTimeData time;
        CCString msg = "[" + time.to_String() + "] -> Failed to update the database -> " + mysql_error(&mysql);
        CC::Println(msg);
        return false;
    }
    return true;
}
