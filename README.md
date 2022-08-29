# 库的制作和使用


## 静态库

### 制作

首先将库的源文件 `add.h` 编译成 `.o` 为后缀的目标文件：

``` bash
# 进入 lib 所在目录
cd lib
# 执行编译
g++ -c add.cpp
```

然后通过 ar 工具生成 `.a` 为后缀的静态库文件：

``` bash
ar -crv libadd.a add.o
```

### 使用

在 `main.cpp` 中引入 `add.h` 头文件：

``` c++
#include <iostream>
#include "add.h"

int main() {
    std::cout  << add(1,2) << std::endl;
}
```

编译 `main.cpp` 并链接静态库 `libadd.a`：

``` bash
# 回退到 main.cpp 所在目录
cd ..
# 执行编译链接
# -I：指定头文件的搜索目录，默认会优先从 -I 指定的目录开始搜索头文件
# -L：指定静态库的搜索目录
# -l：指定链接的静态库，-ladd 会链接 libadd.a 静态库，-l 后可以省略 lib 前缀和 .a 后缀
g++ main.cpp -Ilib -Llib -ladd -o main

# 执行
./main
```


## 动态库


### 制作

制作动态库和静态库的过程类型，首先编译成 `.o` 为后缀的目标文件：

``` bash
# 进入 lib 所在目录
cd lib

# 执行编译
# -fPIC：要求编译生成与绝对地址无关的程序
g++ -fPIC -c add.cpp
```


然后生成 `.so` 为后缀的动态库：


``` bash
# -shared：指定生成动态链接库
g++ -shared -o libadd.so add.o
```

### 使用

`main.cpp` 不需要任何改动，和引入静态链接库一样。执行

``` bash
g++ main.cpp -Ilib -Llib -ladd -o main

# 执行
./main
```

此时发现执行代码报错，如下所示：

``` bash
dyld[1605]: Library not loaded: libadd.so
  Referenced from: /Users/xxx/Desktop/xxx/embed-v8/main
  Reason: tried: 'libadd.so' (no such file), '/usr/local/lib/libadd.so' (no such file), '/usr/lib/libadd.so' (no such file), '/Users/xxx/Desktop/xxx/embed-v8/libadd.so' (no such file), '/usr/local/lib/libadd.so' (no such file), '/usr/lib/libadd.so' (no such file)
Abort trap: 6
```

上述报错是说明没有找到被链接的动态库地址，同时提示我们它寻找动态库地址的规则：

``` bash
'/usr/local/lib/libadd.so'
'/usr/lib/libadd.so'
# 猜测 main.cpp 可执行文件所在的目录
'/Users/xxx/Desktop/xxx/embed-v8/libadd.so'
```

将 `embed-v8/lib/libadd.so` 复制到 `embed-v8/libadd.so`，并重新执行：

``` bash
cp lib/libadd.so libadd.so

# 执行
./main
```

执行成功。