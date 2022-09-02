# V8 静态库的使用


> 温馨提示：由于 V8 源码和制作的静态库过于庞大，因此这里不做源码展示和上传处理。

## 目录结构

```		
└── v8/												
    ├── include/ 												# V8 库的头文件目录
    ├── out.gn/x64.release.sample/obj/ 	# 编译的目标目录	
    └── samples/												# 示例代码目录
```

## 执行编译

``` bash
# 进入 v8 目录，执行编译链接
g++ -I. -Iinclude samples/hello-world.cc -o hello_world -lv8_monolith -Lout.gn/x64.release.sample/obj/ -pthread -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX
# 执行文件
./hello_world
# 输出
Hello, World!
3 + 4 = 7
```