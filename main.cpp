// 包含库的头文件，使用 <>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 包含自定义的头文件，用 ""
#include "libplatform/libplatform.h"
#include "v8.h"

// 引入自定义的 utils.h 头文件
#include "utils.h"

int main(int argc, char* argv[]) {
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
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

    /**
     1. 创建一个上下文，执行 micro1.js 和 micro2.js
    */

    {

      // Create a new context.
      // 创建一个上下文
      v8::Local<v8::Context> context = v8::Context::New(isolate);

      // Enter the context for compiling and running the script.
      // 进入该上下文执行 JS 代码
      v8::Context::Scope context_scope(context);

      {
        // 读取当前 main.cpp 同级目录下的 micro1.js 文件
        const char* code = readJavaScriptFile("micro1.js");

        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(isolate, code).ToLocalChecked();

        // Compile the source code.
        v8::Local<v8::Script> script =
            v8::Script::Compile(context, source).ToLocalChecked();

        // Run the script to get the result.
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

        // Convert the result to an UTF8 string and print it.
        v8::String::Utf8Value utf8(isolate, result);
        printf("%s\n", *utf8);
      }

      {
        // 读取当前 main.cpp 同级目录下的 micro2.js 文件
        const char* code = readJavaScriptFile("micro2.js");

        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source =
            v8::String::NewFromUtf8(isolate, code).ToLocalChecked();

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
    
    /**
     2. 创建两个上下文，分别执行 micro1.js 和 micro2.js
    */

    {
      // Create a new context.
      // 创建一个新的上下文
      v8::Local<v8::Context> context = v8::Context::New(isolate);

      // Enter the context for compiling and running the hello world script.
      // 进入新的上下文执行 JS 代码
      v8::Context::Scope context_scope(context);

      // 读取当前 main.cpp 同级目录下的 micro1.js 文件
      const char* code = readJavaScriptFile("micro1.js");

      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8(isolate, code).ToLocalChecked();

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }


    {
      // Create a new context.
      // 创建一个新的上下文
      v8::Local<v8::Context> context = v8::Context::New(isolate);

      // Enter the context for compiling and running the hello world script.
      // 进入新的上下文执行 JS 代码
      v8::Context::Scope context_scope(context);

      // 读取当前 main.cpp 同级目录下的 micro2.js 文件
      const char* code = readJavaScriptFile("micro2.js");

      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8(isolate, code).ToLocalChecked();

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