# V8 示例

> 温馨提示：该示例采用 Homebrew 下载的 V8 动态库进行演示，版本为 10.2.154.4。


该示例的功能：读取 `hello-world.js` 并执行文件中的 JavaScript 代码。


## g++ 多文件编译


``` bash
cd lib
# -c 不进行链接
g++ -c  -o file.o file.cpp 
cd ..
g++ main.cpp utils/file.o  -o main -Iutils -Iv8/include -Lv8 -lv8 -lv8_libplatform -std=c++17 -DV8_COMPRESS_POINTERS
# 执行
./main
```
