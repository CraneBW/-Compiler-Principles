/**
 * @file symbol_table.cpp
 * @brief 符号表类的实现
 */

#include "symbol_table.h"
#include <iostream>

// 符号类实现
Symbol::Symbol()
    : name(""), type(TYPE_UNKNOWN), address(0), isConstant(false) {}

Symbol::Symbol(const std::string& name, SymbolType type, int address, bool isConstant)
    : name(name), type(type), address(address), isConstant(isConstant) {}

std::string Symbol::getName() const {
    return name;
}

SymbolType Symbol::getType() const {
    return type;
}

void Symbol::setType(SymbolType type) {
    this->type = type;
}

int Symbol::getAddress() const {
    return address;
}

void Symbol::setAddress(int address) {
    this->address = address;
}

bool Symbol::isConst() const {
    return isConstant;
}

void Symbol::setConst(bool isConst) {
    isConstant = isConst;
}

// 符号表类实现
SymbolTable::SymbolTable() : currentAddress(0) {}

bool SymbolTable::addSymbol(const std::string& name, SymbolType type, bool isConstant) {
    if (exists(name)) {
        return false;  // 符号已存在
    }
    
    // 创建新符号并设置地址
    Symbol symbol(name, type, currentAddress, isConstant);
    symbols[name] = symbol;
    
    // 地址增加（可根据具体类型设置不同的增量）
    currentAddress += 4;  // 假设每个符号占用4个字节
    
    return true;
}

Symbol* SymbolTable::findSymbol(const std::string& name) {
    if (exists(name)) {
        return &symbols[name];
    }
    return nullptr;
}

bool SymbolTable::exists(const std::string& name) const {
    return symbols.find(name) != symbols.end();
}

std::vector<Symbol> SymbolTable::getAllSymbols() const {
    std::vector<Symbol> result;
    for (const auto& pair : symbols) {
        result.push_back(pair.second);
    }
    return result;
}

void SymbolTable::print() const {
    std::cout << "===== 符号表 =====" << std::endl;
    std::cout << "名称\t类型\t地址\t是否常量" << std::endl;
    
    for (const auto& pair : symbols) {
        const Symbol& symbol = pair.second;
        std::string typeStr;
        
        switch (symbol.getType()) {
            case TYPE_INTEGER:
                typeStr = "INTEGER";
                break;
            case TYPE_LONG:
                typeStr = "LONG";
                break;
            case TYPE_PROCEDURE:
                typeStr = "PROCEDURE";
                break;
            default:
                typeStr = "UNKNOWN";
        }
        
        std::cout << symbol.getName() << "\t"
                  << typeStr << "\t"
                  << symbol.getAddress() << "\t"
                  << (symbol.isConst() ? "是" : "否") << std::endl;
    }
    
    std::cout << "==================" << std::endl;
}