/**
 * @file lexer.h
 * @brief 词法分析器类的定义
 */

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "token.h"
#include "symbol_table.h"

/**
 * @brief 词法分析器类
 */
class Lexer {
private:
    std::string sourceCode;     // 源代码
    int position;               // 当前位置
    int line;                   // 当前行号
    int column;                 // 当前列号
    char currentChar;           // 当前字符
    SymbolTable symbolTable;    // 符号表
    
    // 关键字映射表
    std::map<std::string, TokenType> keywords;
    
    /**
     * @brief 初始化关键字映射表
     */
    void initKeywords();
    
    /**
     * @brief 前进一个字符
     */
    void advance();
    
    /**
     * @brief 跳过空白字符
     */
    void skipWhitespace();
    
    /**
     * @brief 跳过注释
     */
    void skipComment();
    
    /**
     * @brief 识别数字
     * @return 数字Token
     */
    Token number();
    
    /**
     * @brief 识别标识符或关键字
     * @return 标识符或关键字Token
     */
    Token identifier();
    
    /**
     * @brief 查看下一个字符（不前进）
     * @return 下一个字符
     */
    char peek();

public:
    /**
     * @brief 构造函数
     * @param source 源代码
     */
    Lexer(const std::string& source);
    
    /**
     * @brief 从文件构造
     * @param filename 源代码文件名
     */
    Lexer(std::ifstream& file);
    
    /**
     * @brief 获取下一个Token
     * @return 下一个Token
     */
    Token getNextToken();
    
    /**
     * @brief 获取所有Token
     * @return Token列表
     */
    std::vector<Token> getAllTokens();
    
    /**
     * @brief 获取符号表
     * @return 符号表
     */
    SymbolTable getSymbolTable() const;
};

#endif // LEXER_H 