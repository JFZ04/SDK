#include "CCSQLite3.h"

CCSQLite3::CCSQLite3(CCString Path) {
    Open(Path);
}

bool CCSQLite3::Open(CCString Path) {
    int ret = sqlite3_open(Path.c_str(),&SQL);
    if (ret)
    {
        return false;
    }
    return true;
}

bool CCSQLite3::SqlUpdate(CCString sql) {
    return SQlite(sql.c_str(), nullptr, nullptr);
}

CCResultSet CCSQLite3::SqlQuery(CCString sql) {
    char** pResult;
    int nRow;
    int nCol = 0;
    char* errmsg = nullptr;
    CCResultSet Row_Col;
    int nResult = sqlite3_get_table(SQL, sql.c_str(), &pResult, &nRow, &nCol, &errmsg);
    if (nResult != SQLITE_OK)
    {
        sqlite3_free(errmsg);
        return { };
    }
    Row_Col.Col = nCol;
    Row_Col.Row = nRow;
    int nIndex = nCol;
    for (int i = 0; i < nRow; i++)
    {
        JSON strCol;
        for (int j = 0; j < nCol; j++)
        {
            bool is_int = containsDecimal(pResult[nIndex]);
            try {
                // 根据是否有小数点选择转换类型
                if (is_int) {
                    auto result = convert<double>(pResult[nIndex]);
                    strCol[pResult[j]] = result;
                } else {
                    int result = convert<int>(pResult[nIndex]);
                    strCol[pResult[j]] = result;
                }

            }
            catch (std::exception &e){
                strCol[pResult[j]] = pResult[nIndex];
            }
            ++nIndex;
        }
        Row_Col.Data.push_back(strCol);
    }
    sqlite3_free_table(pResult);
    Row_Col.Flag = true;
    return Row_Col;
}

bool CCSQLite3::Close(){
    return sqlite3_close(SQL) != 0;
}

bool CCSQLite3::SQlite(const char* sql,int(*callback)(void *,int,char**,char**),void* a){
    mutex.lock();
    int rc = sqlite3_exec(SQL, sql, callback, a, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(zErrMsg);
        mutex.unlock();
        return false;
    }
    else
    {
        mutex.unlock();
        return true;
    }
}

CCString CCSQLite3::GetError() {
    return zErrMsg;
}