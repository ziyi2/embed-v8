# V8 示例

> 温馨提示：该示例采用 Homebrew 下载的 V8 动态库进行演示，版本为 11.1.277.17。

该示例的功能：读取 `micro1.js` 和 `micrl2.js` 并执行文件中的 JavaScript 代码。

## V8 动态库下载

``` bash
# 下载 Homebrew（需要科学上网）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# 下载 v8
brew install v8

# 下载的打印信息
# Running `brew update --auto-update`...
# ==> Downloading https://formulae.brew.sh/api/formula.jws.json
# ######################################################################## 100.0%
# ==> Downloading https://formulae.brew.sh/api/cask.jws.json
# ######################################################################## 100.0%
# ==> Fetching v8
# ==> Downloading https://ghcr.io/v2/homebrew/core/v8/manifests/11.1.277.17
# ######################################################################## 100.0%
# ==> Downloading https://ghcr.io/v2/homebrew/core/v8/blobs/sha256:ba5810b3e521e298204dbbf8b97fcf1f7d8d95c2567fdcce3bf50b43cb6840bf
# ==> Downloading from https://pkg-containers.githubusercontent.com/ghcr1/blobs/sha256:ba5810b3e521e298204dbbf8b97fcf1f7d8d95c2567fdcce3bf50b43cb6840bf?se=2023-04-03T12%3A55%3A00Z&sig=f%2BQeNhg5nlDQz
# ######################################################################## 100.0%
# ==> Pouring v8--11.1.277.17.arm64_ventura.bottle.tar.gz
# 🍺  /opt/homebrew/Cellar/v8/11.1.277.17: 196 files, 49.8MB
# ==> Running `brew cleanup v8`...
# Disable this behaviour by setting HOMEBREW_NO_INSTALL_CLEANUP.
# Hide these hints with HOMEBREW_NO_ENV_HINTS (see `man brew`).

# 拷贝 V8 到当前目录下
cp -R /opt/homebrew/Cellar/v8/11.1.277.17/  ./v8
```


## g++ 多文件编译


``` bash
cd utils
# -c 不进行链接
g++ -c  -o file.o file.cpp 
cd ..
# 编译，会在当前根目录下生成 main 可执行文件
# -I：搜索头文件时搜索当前项目的 v8 目录下的头文件，因此 include/v8.h 可以被识别
# -L：指定动态库的搜索目录为 v8
# -l：链接 v8/lib 目录下的 libv8.dylib 和 libv8_libplatform.dylib 两个动态库
# -std：使用 c++17 标准
# -D：编译时宏定义 V8_COMPRESS_POINTERS，开启 V8 的指针压缩
g++ main.cpp utils/file.o  -o main -Iutils -Iv8/include -Lv8/lib -lv8 -lv8_libplatform -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX
# 执行

./main
```


## CMake 多文件编译

当 `utils` 目录下的文件变多以后，一条条执行内部的 `.cpp` 文件非常低效，因此可以使用 CMake 来简化编译工作，在根目录下配置 `CMakeLists.txt`：

```
# 最低要求版本
cmake_minimum_required(VERSION 3.2)

# 设置项目名称
project(main)

# 设置 C++17 标准，类似于 g++ 中的 -std 参数
set(CMAKE_CXX_STANDARD 17)

# 将指定 utils 目录下的源文件存放在变量 UTILS 中
aux_source_directory(utils UTILS)

# 设置生成的可执行文件，类似于 g++ 中的 -o 参数
add_executable(main main.cpp ${UTILS})

# 设置 V8 头文件的搜索目录，类似于 g++ 中的 -I 参数
include_directories(v8/include)

# 设置头文件搜索 utils 目录
include_directories(utils)

# 设置 V8 的预处理宏定义，类似于 g++ 中的 -D 参数
target_compile_definitions(main PRIVATE V8_COMPRESS_POINTERS)

# 设置动态库的查找地址，类似于 g++ 中的 -L 参数
target_link_directories(main PRIVATE v8/lib)

# 设置需要链接的 V8 动态库，类似于 g++ 中的 -l 参数
target_link_libraries(main PRIVATE v8 v8_libplatform)
```

``` bash
# 进入项目根目录，生成 MakeFile
cmake .
# 编译，会在当前根目录下生成 main 可执行文件
make
# 执行
./main
# 打印
```