# 最低要求版本
cmake_minimum_required(VERSION 3.2)

# 设置项目名称
project(main)

# 设置 C++14 保准，类似于 g++ 中的 -std 参数
set(CMAKE_CXX_STANDARD 14)

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