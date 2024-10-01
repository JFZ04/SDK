#ifndef IMGUICPPDEMO_CCIOS_H
#define IMGUICPPDEMO_CCIOS_H
#include "cstdio"
#include "dirent.h"
#include "vector"
#include "CCString.h"
#include "mutex"
#include <unistd.h> // 包含 _access 和 mkdir
#include <sys/stat.h> // 包含 mkdir 的权限模式
#include "CCByteArray.h"

namespace CC
{
    typedef int RIos;
    enum ios
    {
        r,
        w,
        a,
        rb,
        wb,
        ab,
        Addr,
        Addw,
        Adda,
        Addrb,
        Addwb,
        Addab,
    };
}

struct FileInfo
{
    CCString Path;
    CCString Name;
    long long Size = 0;
    bool Directory = false;
};


class CCFile
{
public:
    CCFile() = default;
    explicit CCFile(const char* path,CC::RIos mode = CC::ios::rb,bool load = true);
    void Open(const char* path,CC::RIos mode = CC::ios::rb,bool load = true);
    bool IsOpen() const;
    void Close();
    bool Read(char *buffer,int n);
    bool Read(void *buffer,int n,int size);
    bool Write(const char *buffer);
    bool Write(void *buffer,int n,int size = 1);
    bool Deleted(bool isDelete = false);
    bool IsDirectory();
    FileInfo GetFileInfo();
    std::vector<FileInfo> GetDirectoryList(bool F = true);
    void Move(CCString Path);
    bool Copy(CCString Path);
    static bool CreateFolder(CCString Path);
    static CCString GetnormalizePath(CCString path);
    static bool Revise(CCString OldName,CCString NewName);

private:
    FileInfo info;
    bool bin = false,F = true;
    FILE * file = nullptr;
    const char* mod[13] = {"r","w","a","rb","wb","ab","r+","w+","a+","rb+","wb+","ab+"};
    CC::RIos Mode = CC::ios::r;
    std::mutex mtx;


private:
    void rmtree(const char *path);
    int is_directory(const char *path);
    long long GetFileSize(const char *filename);
    const char *GetFileName(const char *path);
    std::vector<FileInfo> GetFileList(const char *path);
    static std::string normalizePath(const std::string &path);
    void move_file(const char *src, const char *dst);
    int copy_file(const char *source, const char *destination);
    static int create_fun(char*filepath);

};


#endif
