/**
 * @file token.h
 * @brief 定义Token类，表示词法单元
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/**
 * Token类型枚举
 */
enum TokenType {
    // 保留字
    PROGRAM = 1, CONST = 2, VAR = 3, INTEGER = 4,
    LONG = 5, PROCEDURE = 6, IF = 7, THEN = 8,
    WHILE = 9, DO = 10, READ = 11, WRITE = 12,
    BEGIN = 13, END = 14, ODD = 15,
    
    // 运算符
    PLUS = 16, MINUS = 17, MULTIPLY = 18, DIVIDE = 19,
    EQUAL = 20, NOT_EQUAL = 21, LESS = 22, LESS_EQUAL = 23,
    GREATER = 24, GREATER_EQUAL = 25,
    
    // 分隔符
    DOT = 26, COMMA = 27, SEMICOLON = 28, COLON = 29,
    ASSIGN = 30, LEFT_PAREN = 31, RIGHT_PAREN = 32,
    
    // 其他
    NUMBER = 33, IDENTIFIER = 34, HASH = 35
};

/**
 * @brief Token类，表示词法单元
 */
class Token {
private:
    TokenType type;     // Token类型
    std::string value;  // Token的原始字符串值
    int line;           // 所在行号
    int column;         // 所在列号

public:
    /**
     * @brief 构造函数
     * @param type Token类型
     * @param value Token的原始字符串值
     * @param line 所在行号
     * @param column 所在列号
     */
    Token(TokenType type, const std::string& value, int line, int column);
    
    /**
     * @brief 获取Token类型
     * @return TokenType
     */
    TokenType getType() const;
    
    /**
     * @brief 获取Token的原始字符串值
     * @return 字符串值
     */
    std::string getValue() const;
    
    /**
     * @brief 获取Token的行号
     * @return 行号
     */
    int getLine() const;
    
    /**
     * @brief 获取Token的列号
     * @return 列号
     */
    int getColumn() const;
    
    /**
     * @brief 获取Token类型的名称
     * @return 类型名称
     */
    std::string getTypeName() const;
};

#endif // TOKEN_H 