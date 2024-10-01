#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include "../include/CCFile.h"
#include <iostream>


CCFile::CCFile(const char *path, CC::RIos mode,bool load){
    this->Open(path, mode,load);
}

void CCFile::Open(const char *path, CC::RIos mode,bool load)
{
#ifdef _WIN32
    CCString A = CCByteArray::fromString(path).Format();
    path = A.c_str();
#endif
    Mode = mode;
    if(mode == 3 || mode == 4 || mode == 5 || mode == 9 || mode == 10 || mode == 11){
        bin = true;
    }
    info.Path = normalizePath(path);
    info.Directory = IsDirectory();
    info.Name = (char*)GetFileName(info.Path.c_str());
    if(load){
        file = fopen(info.Path.c_str(),mod[Mode]);
    }
}

bool CCFile::IsOpen() const
{
    if(file == NULL){
        return false;
    }
    else{
        return true;
    }
}
bool CCFile::Read(char *buffer, int n){
    mtx.lock();
    if (fgets(buffer, n, file) == nullptr)
    {
        mtx.unlock();
        return false; // 读取失败
    }
    mtx.unlock();
    return true;
}
bool CCFile::Read(void *buffer, int n, int size){
    mtx.lock();
    size_t elementsRead = fread(buffer, size, n, file);
    if (elementsRead != n) {
        mtx.unlock();
        return false;
    }
    mtx.unlock();
    return elementsRead == n; // 检查是否按预期读取了所有元素
}
bool CCFile::Write(const char *buffer){
    mtx.lock();
    if(fputs(buffer, file) == EOF){
        mtx.unlock();
        return false;
    }
    mtx.unlock();
    return true;
}
bool CCFile::Write(void *buffer, int n, int size){
    size_t writtenElements = fwrite(buffer, size, n, file);
    if (writtenElements != n) {
        std::cout<< "Failed to write all elements." << std::endl;
        return false;
    }
    return true;
}
bool CCFile::Deleted(bool isDelete) {
    if(IsDirectory()){
        if(isDelete){

            rmtree(info.Path.c_str());
            return true;
        }
        else{
            return rmdir(info.Path.c_str())!=-1;
        }
    } else{
        return remove(info.Path.c_str())!=-1;
    }
}
void CCFile::rmtree(const char *path) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
                continue;
            char newpath[1024];
            snprintf(newpath, sizeof(newpath), "%s/%s", path, ent->d_name);
            if (is_directory(newpath)) { // Check if it's a directory
                rmtree(newpath);
            } else {
                remove(newpath);
            }
        }
        closedir(dir);
    }
    rmdir(path);
}

int CCFile::is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return 0;
}

bool CCFile::IsDirectory() {
    return is_directory(info.Path.c_str()) != 0;
}

long long CCFile::GetFileSize(const char *filename) {
    mtx.lock();
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        mtx.unlock();
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return -1;
    }
    // 移动文件指针到文件末尾
    if (fseek(file, 0L, SEEK_END) != 0) {
        mtx.unlock();
        fclose(file);
        return -1;
    }
    // 获取文件指针的位置
    long long size = ftell(file);
    // 关闭文件
    mtx.unlock();
    fclose(file);
    return size;
}

const char *CCFile::GetFileName(const char *path) {
    const char *ptr = strrchr(path, '/');
    if (ptr == NULL) {
        ptr = strrchr(path, '\\'); // 如果没有斜杠，尝试反斜杠
    }
    return (ptr == NULL) ? path : ptr + 1;
}

FileInfo CCFile::GetFileInfo() {
    if(!info.Directory){
        info.Size = GetFileSize(info.Path.c_str());
    }
    return info;
}

std::vector<FileInfo> CCFile::GetFileList(const char *path) {
    std::vector<FileInfo> fileList;
    DIR *dir = opendir(path);
    if (dir != nullptr) {
        while (true) {
            struct dirent *ent = readdir(dir);
            if (ent == nullptr) {
                break;
            }
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 && this->F) {
                continue;
            }
            FileInfo fileInfo;
            fileInfo.Name = ent->d_name;
            fileInfo.Path = CCString(path) + "/" + fileInfo.Name;
            fileInfo.Directory = is_directory(fileInfo.Path.c_str()) != 0;
            if (!fileInfo.Directory) {
                fileInfo.Size = GetFileSize(fileInfo.Path.c_str());
            }
            fileList.push_back(fileInfo);
        }
    }
    closedir(dir);
    return fileList;
}

std::vector<FileInfo> CCFile::GetDirectoryList(bool F) {
    if(IsDirectory()){
        this->F = F;
        return GetFileList(info.Path.c_str());
    }
    else{
        return {};
    }
}

std::string CCFile::normalizePath(const std::string &path) {
    std::string normalized;
    bool lastWasSlash = false; // 标记上一个字符是否为斜杠

    for (char ch : path) {
        if (ch == '/' || ch == '\\') {
            if (!lastWasSlash) {
                normalized += '/';
                lastWasSlash = true;
            }
        } else {
            normalized += ch;
            lastWasSlash = false;
        }
    }

    return normalized;
}

void CCFile::move_file(const char *src, const char *dst) {
    if (rename(src, dst) != 0) {
        perror("Failed to move file");
    }
}

void CCFile::Move(CCString Path) {
    move_file(info.Path.c_str(), Path.c_str());
}

int CCFile::copy_file(const char *source, const char *destination) {
    FILE *in = fopen(source, "rb");
    if (in == NULL) {
        perror("Failed to open source file");
        return 1;
    }

    FILE *out = fopen(destination, "wb");
    if (out == NULL) {
        perror("Failed to open destination file");
        fclose(in);
        return 1;
    }

    char buffer[4096];
    size_t result;

    while ((result = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, result, out) != result) {
            perror("Failed to write to destination file");
            fclose(in);
            fclose(out);
            return 1;
        }
    }

    if (ferror(in)) {
        perror("Error reading from source file");
        fclose(in);
        fclose(out);
        return 1;
    }

    fclose(in);
    fclose(out);

    return 0;
}

bool CCFile::Copy(CCString Path) {
    return copy_file(info.Path.c_str(), Path.c_str()) == 0;
}

void CCFile::Close() {
    fclose(file);
}

int CCFile::create_fun(char *filepath) {
    int res;
    //filepath为绝对路径
    //如果文件夹不存在
#ifdef _WIN32
    if(_access(filepath,0)!=0)
        //创建文件夹
        res = mkdir(filepath);// 返回 0 表示创建成功，-1 表示失败
#elif __linux__
    if(access(filepath,0)!=0)
        //创建文件夹
            res = mkdir(filepath,0775);// 返回 0 表示创建成功，-1 表示失败
#endif

        //remove(filename) 删除文件
    else
        res = 1;
    return res;
}

bool CCFile::CreateFolder(CCString Path) {
    return create_fun((char*)Path.c_str()) == 0;
}

CCString CCFile::GetnormalizePath(CCString path) {
    return normalizePath(path);
}

bool CCFile::Revise(CCString oldname,CCString newname) {
    if (rename(oldname.c_str(), newname.c_str()) != 0) {
        perror("Error renaming file"); // 输出错误信息
        return false;
    }
    return true;
}

