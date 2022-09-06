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


## CMake 多文件编译

当 `utils` 目录下的文件变多以后，一条条执行内部的 `.cpp` 文件非常低效，因此可以使用 CMake 来简化编译工作：

```
# 最低要求版本
cmake_minimum_required(VERSION 3.2)

# 设置项目名称
project(main)

# 设置 C++14 标准，类似于 g++ 中的 -std 参数
set(CMAKE_CXX_STANDARD 17)

# 设置生成的可执行文件，类似于 g++ 中的 -o 参数
add_executable(main main.cpp)

# 设置 V8 头文件的搜索目录，类似于 g++ 中的 -I 参数
include_directories(./v8/include)

# 设置 V8 的预处理宏定义，类似于 g++ 中的 -D 参数
target_compile_definitions(main PRIVATE V8_COMPRESS_POINTERS)

# 设置动态库的查找地址，类似于 g++ 中的 -L 参数
target_link_directories(main PRIVATE ./v8/lib)

# 设置需要链接的 V8 动态库，类似于 g++ 中的 -l 参数
target_link_libraries(main PRIVATE v8 v8_libplatform)
```