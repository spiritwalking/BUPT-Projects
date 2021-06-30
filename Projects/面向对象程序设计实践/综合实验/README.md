# 电商交易平台

### 开发环境

- 操作系统：Windows10 
- 开发平台：Visual Studio Code
- 编程语言及标准：C++11
- 编译器：MinGW
- 编译工具：CMake

### 使用说明

软件运行需要两个文本文件，在版本2、3添加了购物车文本文件。

* 在` userdata.txt`中，你可以按照` 用户名 密码 用户类型 余额`的格式来编写，例如：

```
ycy 240 1 1000
txy 240 0 1234
xmx 240 0 1234
phl 240 1 1234
hyl 520 1 222
```

* 在` commdata.txt`中，你可以按照` 商品名 商品类型 所有者 价格 折扣 数量 描述`的格式来编写，例如：

```
T-shirt cloth phl 135.8 1 1100 Summer-Day!
skirt cloth phl 220 1 1234 for-beautiful-boys
pajamas cloth phl 123 1 2233 love-sleeping
Cocacola food hyl 2.5 1 12345 happy-water
hotpot food hyl 102.2 1 1223 beat-sichuan-food
rose food hyl 3.8 1 203 simple-but-useful
CSAPP book ycy 123.2 0.5 1234 good-book
CPrimer book ycy 1.8 0.5 20000 entry-to-soil
ThreeBody book ycy 12 0.5 1300 best-scifi
```

* 在` cartdata.txt`中，你可以在` 用户名`后加上` 商品名 数量`来编写，例如：

```
txy
rose 10
hotpot 20
xmx
skirt 1000
```

