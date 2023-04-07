#include <iostream>
#include <fstream>
#include "file.h"

using namespace std;

// C++ 读写示例：https://cplusplus.com/doc/tutorial/files/
// 查看最后一个示例 Binary files

char* readJavaScriptFile(const char* fileName) {
    
    char* code;

    // 创建一个 ifstream，并打开名为 fileName 的文件
    // fileName 可以是 string 类型，也可以是一个指向字符串存储地址的指针

    // ios::in：打开文件用于读取
    // ios::ate：文件打开后定位到文件末尾
    // 这里的 | 是一个位运算符，表明上述两个操作都要执行
    ifstream file(fileName, ios::in|ios::ate);

    if(file.is_open()) {
        // 由于打开时定位到末尾，因此可以直接获取文件大小
        streampos size = file.tellg();
        // 开辟和文件大小一致的字符串数组空间，从而可以有足够的空间存放文件的字符串内容
        code = new char[size];
        // 重新定位到文件开头
        file.seekg (0, ios::beg);
        // 读取文件的全部内容（通过 size 指定读取的文件大小）
        file.read (code, size);
        // 关闭文件
        file.close();
    }
    
    
    return code;
}
