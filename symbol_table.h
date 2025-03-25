/**
 * @file symbol_table.h
 * @brief 符号表类的定义
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <map>
#include <vector>

/**
 * @brief 符号类型枚举
 */
enum SymbolType {
    TYPE_UNKNOWN,    // 未知类型
    TYPE_INTEGER,    // 整数类型
    TYPE_LONG,       // 长整数类型
    TYPE_PROCEDURE   // 过程类型
};

/**
 * @brief 符号类
 */
class Symbol {
private:
    std::string name;      // 符号名称
    SymbolType type;       // 符号类型
    int address;           // 符号地址（可选）
    bool isConstant;       // 是否为常量

public:
    /**
     * @brief 默认构造函数
     */
    Symbol();

    /**
     * @brief 构造函数
     * @param name 符号名称
     * @param type 符号类型
     * @param address 符号地址
     * @param isConstant 是否为常量
     */
    Symbol(const std::string& name, SymbolType type = TYPE_UNKNOWN, 
           int address = 0, bool isConstant = false);
    
    /**
     * @brief 获取符号名称
     * @return 符号名称
     */
    std::string getName() const;
    
    /**
     * @brief 获取符号类型
     * @return 符号类型
     */
    SymbolType getType() const;
    
    /**
     * @brief 设置符号类型
     * @param type 符号类型
     */
    void setType(SymbolType type);
    
    /**
     * @brief 获取符号地址
     * @return 符号地址
     */
    int getAddress() const;
    
    /**
     * @brief 设置符号地址
     * @param address 符号地址
     */
    void setAddress(int address);
    
    /**
     * @brief 判断是否为常量
     * @return 是否为常量
     */
    bool isConst() const;
    
    /**
     * @brief 设置是否为常量
     * @param isConst 是否为常量
     */
    void setConst(bool isConst);
};

/**
 * @brief 符号表类
 */
class SymbolTable {
private:
    std::map<std::string, Symbol> symbols;   // 符号映射表
    int currentAddress;                       // 当前地址

public:
    /**
     * @brief 构造函数
     */
    SymbolTable();
    
    /**
     * @brief 添加符号
     * @param name 符号名称
     * @param type 符号类型
     * @param isConstant 是否为常量
     * @return 是否添加成功
     */
    bool addSymbol(const std::string& name, SymbolType type = TYPE_UNKNOWN, bool isConstant = false);
    
    /**
     * @brief 查找符号
     * @param name 符号名称
     * @return 符号指针（不存在则返回nullptr）
     */
    Symbol* findSymbol(const std::string& name);
    
    /**
     * @brief 判断符号是否存在
     * @param name 符号名称
     * @return 是否存在
     */
    bool exists(const std::string& name) const;
    
    /**
     * @brief 获取所有符号
     * @return 符号列表
     */
    std::vector<Symbol> getAllSymbols() const;
    
    /**
     * @brief 打印符号表
     */
    void print() const;
};

#endif // SYMBOL_TABLE_H 