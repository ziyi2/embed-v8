# V8 静态库的使用

该静态库通过官方的 V8 文档检出 V8 源码构建生成，版本为 10.7.75，为当时最新的检出版本。

> 温馨提示：由于检出的 V8 源码有将近 16 G 大小，因此这里只上传了头文件目录、GN 编译结果目录以及 V8 的示例目录。

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