# V8 动态库的使用

使用 brew 下载 [V8](https://formulae.brew.sh/formula/v8)，本项目下载的版本为 10.2.154.4。


## 目录结构

``` 
├── v8 						# 库源码
│   ├── inclide/ 	# 头文件
│   └── lib/			# 动态库
└── main.cpp 			# 应用源码
```

## 引入使用

将 V8 官方的 10.2.154.4 版本的 [Hello World Example](https://github.com/v8/v8/blob/10.2.154.4/samples/hello-world.cc) 引入 `main.cpp` 使用，例如

``` c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 不需要使用 include/libplatform/libplatform.h
#include "libplatform/libplatform.h"

// #include "include/v8-context.h"
// #include "include/v8-initialization.h"
// #include "include/v8-isolate.h"
// #include "include/v8-local-handle.h"
// #include "include/v8-primitive.h"

// 这里简单起见，引入包含上述头文件的 V8 库的头文件
// 注意在编译时会指定头文件的搜索目录 -I 为 ./v8/include，因此不需要使用 include/v8.h
#include "v8.h"

int main(int argc, char* argv[]) {
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
#ifdef V8_SANDBOX
  if (!v8::V8::InitializeSandbox()) {
    fprintf(stderr, "Error initializing the V8 sandbox\n");
    return 1;
  }
#endif
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);

    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);

    {
      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "'Hello' + ', World!'");

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}
```

> 温馨提示：去除了 WebAssembly 的示例代码。


将 `mian.cpp` 链接到 V8 动态库，执行编译命令：

``` bash
# 编译
# -I：搜索头文件时搜索当前项目的 v8 目录下的头文件，因此 include/v8.h 可以被识别
# -L：指定动态库的搜索目录为 v8
# -l：链接 lib 目录下的 libv8.dylib 和 libv8_libplatform.dylib 两个动态库
# -std：使用 c++14 标准
# -D：编译时宏定义 V8_COMPRESS_POINTERS，开启 V8 的指针压缩
g++ main.cpp -o main -Iv8/include -Lv8 -lv8 -lv8_libplatform -std=c++14 -DV8_COMPRESS_POINTERS
# 执行
./main
Hello, World!
```

当 C++ 项目越来越庞大时，使用单条 g++ 命令可能无法满足我们的开发诉求，在 C++ 中可以使用 [CMake](https://cmake.org/)，首先需要安装 CMake：

- 方式一：[官方下载地址](https://cmake.org/download/)
- 方式二：使用 Homebrew 安装

本项目使用 Homebrew 的安装命令 `brew install cmake` 安装一键搞定。

> 温馨提示：例如 TypeScript 中的 tsc 命令，当项目复杂后，需要使用类似 Gulp 或者 Webpack 的编译工具。


安装完成后，在项目根目录下新建 `CMakeLists.txt` 配置文件，配置如下：

``` txt
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
```

进行编译和执行：

```bash
# 生成 MakeFile
cmake .
# 编译
make
# 执行
./main
Hello, World!
```