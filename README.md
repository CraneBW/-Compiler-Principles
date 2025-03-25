# Pascal子集词法分析器

## 项目概述

这是一个用C++实现的Pascal子集词法分析器，可以将Pascal子集源代码转换为Token序列，并生成符号表。

## 目录结构

- `token.h/cpp`: Token类的定义和实现
- `symbol_table.h/cpp`: 符号表类的定义和实现
- `lexer.h/cpp`: 词法分析器类的定义和实现
- `main.cpp`: 主程序入口
- `Makefile`: 构建脚本
- `example.pas`: 示例Pascal子集源代码

## 功能特点

- 支持Pascal子集语言的词法分析
- 识别关键字、标识符、运算符、分隔符和数字
- 生成Token序列，格式为`<内码, 单词>`
- 维护标识符符号表
- 支持文件输入输出

## 词法规则

词法分析器根据Pascal子集语言的语法规则识别以下类型的词法单元：

1. **关键字**: PROGRAM, CONST, VAR, INTEGER, LONG, PROCEDURE, IF, THEN, WHILE, DO, READ, WRITE, BEGIN, END, ODD
2. **运算符**: +, -, *, /, =, <>, <, <=, >, >=, :=
3. **分隔符**: ., ,, ;, :, (, )
4. **标识符**: 以字母开头，后跟字母、数字或下划线的序列
5. **无符号整数**: 数字序列

## 编译和使用

### 编译

```bash
make
```

### 运行

```bash
./lexer <输入文件> [输出文件]
```

参数说明：
- `<输入文件>`: Pascal子集源代码文件路径
- `[输出文件]`: 输出文件路径（可选，默认输出到控制台）

### 示例

```bash
./lexer example.pas output.txt
```

## 输出格式

### Token序列

```
===== Token序列 =====
     行号      列号           类型           内码                 值
        1         1        PROGRAM             1            PROGRAM
        1        10    IDENTIFIER            34            Example
        1        17     SEMICOLON            28                  ;
        ...
```

### 符号表

```
===== 符号表 =====
                名称            类型            地址        是否常量
             Example        UNKNOWN              0             否
                MAX        UNKNOWN              4             否
                ...
```

## 注意事项

- 本词法分析器仅支持Pascal子集语言，不支持完整的Pascal语言。
- 错误处理仅包含基本的未知字符识别，不包含更复杂的错误恢复机制。 