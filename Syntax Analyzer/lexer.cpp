#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string& filename) : lineNo(1), columnNo(0) {
    input.open(filename);
    if (!input.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    
    // 读取第一行
    if (!std::getline(input, currentLine)) {
        currentChar = '\0';
    } else {
        // 初始化当前字符为第一行的第一个字符，或者EOF
        if (currentLine.empty()) {
            currentChar = '\0';
        } else {
            currentChar = currentLine[0];
        }
    }
}

Lexer::~Lexer() {
    if (input.is_open()) {
        input.close();
    }
}

void Lexer::advance() {
    columnNo++;
    if (columnNo >= currentLine.length()) {
        if (std::getline(input, currentLine)) {
            columnNo = 0;
            lineNo++;
        } else {
            currentChar = '\0';
            return;
        }
    }
    currentChar = currentLine[columnNo];
}

void Lexer::skipWhitespace() {
    while (currentChar && std::isspace(currentChar)) {
        advance();
    }
}

void Lexer::skipComment() {
    while (currentChar && currentChar != '}') {
        advance();
    }
    if (currentChar == '}') {
        advance();
    }
}

Token Lexer::getNumber() {
    std::string num;
    Token token;
    token.code = 33; // 无符号整数
    token.lineNo = lineNo;
    token.columnNo = columnNo;
    
    while (currentChar && std::isdigit(currentChar)) {
        num += currentChar;
        advance();
    }
    
    token.value = num;
    return token;
}

Token Lexer::getIdentifier() {
    std::string identifier;
    Token token;
    token.lineNo = lineNo;
    token.columnNo = columnNo;
    
    while (currentChar && (std::isalnum(currentChar) || currentChar == '_')) {
        identifier += currentChar;
        advance();
    }
    
    // 关键字识别
    if (identifier == "PROGRAM") token.code = 1;
    else if (identifier == "CONST") token.code = 2;
    else if (identifier == "VAR") token.code = 3;
    else if (identifier == "PROCEDURE") token.code = 6;
    else if (identifier == "BEGIN") token.code = 13;
    else if (identifier == "END") token.code = 14;
    else if (identifier == "IF") token.code = 7;
    else if (identifier == "THEN") token.code = 8;
    else if (identifier == "WHILE") token.code = 9;
    else if (identifier == "DO") token.code = 10;
    else if (identifier == "READ") token.code = 11;
    else if (identifier == "WRITE") token.code = 12;
    else if (identifier == "ODD") token.code = 15;
    else if (identifier == "INTEGER") token.code = 40;
    else if (identifier == "LONG") token.code = 50;
    else token.code = 34; // 标识符
    
    token.value = identifier;
    return token;
}

Token Lexer::getOperator() {
    Token token;
    token.lineNo = lineNo;
    token.columnNo = columnNo;
    
    switch (currentChar) {
        case '+':
            token.code = 16;
            token.value = "+";
            break;
        case '-':
            token.code = 17;
            token.value = "-";
            break;
        case '*':
            token.code = 18;
            token.value = "*";
            break;
        case '/':
            token.code = 19;
            token.value = "/";
            break;
        case '=':
            token.code = 20;
            token.value = "=";
            break;
        case '<':
            advance();
            if (currentChar == '>') {
                token.code = 21;
                token.value = "<>";
                advance();
            } else if (currentChar == '=') {
                token.code = 23;
                token.value = "<=";
                advance();
            } else {
                token.code = 22;
                token.value = "<";
            }
            return token;
        case '>':
            advance();
            if (currentChar == '=') {
                token.code = 25;
                token.value = ">=";
                advance();
            } else {
                token.code = 24;
                token.value = ">";
            }
            return token;
        case ':':
            advance();
            if (currentChar == '=') {
                token.code = 30;
                token.value = ":=";
                advance();
            } else {
                token.code = 29;
                token.value = ":";
            }
            return token;
        default:
            throw std::runtime_error("非预期的运算符");
    }
    
    advance();
    return token;
}

Token Lexer::getDelimiter() {
    Token token;
    token.lineNo = lineNo;
    token.columnNo = columnNo;
    
    switch (currentChar) {
        case '(':
            token.code = 31;
            token.value = "(";
            break;
        case ')':
            token.code = 32;
            token.value = ")";
            break;
        case ',':
            token.code = 27;
            token.value = ",";
            break;
        case ';':
            token.code = 28;
            token.value = ";";
            break;
        case '.':
            token.code = 26;
            token.value = ".";
            break;
        default:
            throw std::runtime_error("非预期的分隔符");
    }
    
    advance();
    return token;
}

Token Lexer::nextToken() {
    while (currentChar) {
        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }
        
        if (currentChar == '{') {
            advance();
            skipComment();
            continue;
        }
        
        if (std::isdigit(currentChar)) {
            return getNumber();
        }
        
        if (std::isalpha(currentChar)) {
            return getIdentifier();
        }
        
        if (currentChar == '+' || currentChar == '-' || currentChar == '*' || 
            currentChar == '/' || currentChar == '=' || currentChar == '<' || 
            currentChar == '>' || currentChar == ':') {
            return getOperator();
        }
        
        if (currentChar == '(' || currentChar == ')' || currentChar == ',' || 
            currentChar == ';' || currentChar == '.') {
            return getDelimiter();
        }
        
        throw std::runtime_error("非预期的字符");
    }
    
    // 文件结束
    Token token;
    token.code = 0;
    token.value = "EOF";
    token.lineNo = lineNo;
    token.columnNo = columnNo;
    return token;
}

void Lexer::reset() {
    input.clear();
    input.seekg(0);
    lineNo = 1;
    columnNo = 0;
    currentLine.clear();
    advance();
} 