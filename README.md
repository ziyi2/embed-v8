# V8 动态库的使用

使用 brew 下载 [V8](https://formulae.brew.sh/formula/v8)，本项目下载的版本为 10.2.154.4。


## 目录结构

``` 
├── v8 						# 库源码
│   ├── inclide/ 	# 头文件
│   ├── lib/		  # 动态库
└── main.cpp 			# 应用源码
```

## 引入使用

将 V8 官方的 10.2.154.4 版本的 [Hello World Example](https://github.com/v8/v8/blob/10.2.154.4/samples/hello-world.cc) 引入 main.cpp 使用，例如

``` c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/libplatform/libplatform.h"

// #include "include/v8-context.h"
// #include "include/v8-initialization.h"
// #include "include/v8-isolate.h"
// #include "include/v8-local-handle.h"
// #include "include/v8-primitive.h"

// 这里简单起见，引入包含上述头文件的 V8 头文件
#include "include/v8.h"

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
g++ main.cpp -o main -Iv8 -Lv8 -lv8 -lv8_libplatform -std=c++14 -DV8_COMPRESS_POINTERS
# 执行
./main
```
