/**
 * @file token.cpp
 * @brief Token类的实现
 */

#include "token.h"
#include <map>

Token::Token(TokenType type, const std::string& value, int line, int column)
    : type(type), value(value), line(line), column(column) {}

TokenType Token::getType() const {
    return type;
}

std::string Token::getValue() const {
    return value;
}

int Token::getLine() const {
    return line;
}

int Token::getColumn() const {
    return column;
}

std::string Token::getTypeName() const {
    static const std::map<TokenType, std::string> tokenNames = {
        {PROGRAM, "PROGRAM"}, {CONST, "CONST"}, {VAR, "VAR"}, 
        {INTEGER, "INTEGER"}, {LONG, "LONG"}, {PROCEDURE, "PROCEDURE"}, 
        {IF, "IF"}, {THEN, "THEN"}, {WHILE, "WHILE"}, 
        {DO, "DO"}, {READ, "READ"}, {WRITE, "WRITE"}, 
        {BEGIN, "BEGIN"}, {END, "END"}, {ODD, "ODD"}, 
        {PLUS, "+"}, {MINUS, "-"}, {MULTIPLY, "*"}, 
        {DIVIDE, "/"}, {EQUAL, "="}, {NOT_EQUAL, "<>"}, 
        {LESS, "<"}, {LESS_EQUAL, "<="}, {GREATER, ">"}, 
        {GREATER_EQUAL, ">="}, {DOT, "."}, {COMMA, ","}, 
        {SEMICOLON, ";"}, {COLON, ":"}, {ASSIGN, ":="}, 
        {LEFT_PAREN, "("}, {RIGHT_PAREN, ")"}, {NUMBER, "无符号整数"}, 
        {IDENTIFIER, "标识符"}, {HASH, "#"}
    };
    
    auto it = tokenNames.find(type);
    if (it != tokenNames.end()) {
        return it->second;
    }
    return "未知";
} 