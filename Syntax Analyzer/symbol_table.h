#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>

// 符号表项结构
struct SymbolTableEntry {
    std::string type;      // 类型：CONST, VAR, PROCEDURE
    std::string value;     // 值（对于常量）
    std::string scope;     // 作用域
    int level;            // 嵌套层次
    bool isParameter;     // 是否是参数
};

#endif // SYMBOL_TABLE_H 