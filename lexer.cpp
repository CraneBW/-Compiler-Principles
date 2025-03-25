/**
 * @file lexer.cpp
 * @brief 词法分析器的实现
 */

#include "lexer.h"
#include <iostream>
#include <cctype>
#include <sstream>

// 初始化关键字映射表
void Lexer::initKeywords() {
    keywords["PROGRAM"] = PROGRAM;
    keywords["CONST"] = CONST;
    keywords["VAR"] = VAR;
    keywords["INTEGER"] = INTEGER;
    keywords["LONG"] = LONG;
    keywords["PROCEDURE"] = PROCEDURE;
    keywords["IF"] = IF;
    keywords["THEN"] = THEN;
    keywords["WHILE"] = WHILE;
    keywords["DO"] = DO;
    keywords["READ"] = READ;
    keywords["WRITE"] = WRITE;
    keywords["BEGIN"] = BEGIN;
    keywords["END"] = END;
    keywords["ODD"] = ODD;
}

Lexer::Lexer(const std::string& source) 
    : sourceCode(source), position(0), line(1), column(1) {
    // 初始化关键字表
    initKeywords();
    
    // 初始化当前字符
    if (!sourceCode.empty()) {
        currentChar = sourceCode[0];
    } else {
        currentChar = '\0';  // 空字符表示结束
    }
}

Lexer::Lexer(std::ifstream& file) {
    // 从文件读取源代码
    std::stringstream buffer;
    buffer << file.rdbuf();
    sourceCode = buffer.str();
    
    // 初始化位置和当前字符
    position = 0;
    line = 1;
    column = 1;
    
    // 初始化关键字表
    initKeywords();
    
    // 初始化当前字符
    if (!sourceCode.empty()) {
        currentChar = sourceCode[0];
    } else {
        currentChar = '\0';  // 空字符表示结束
    }
}

void Lexer::advance() {
    // 前进一个字符，更新位置和行列信息
    position++;
    column++;
    
    if (position >= sourceCode.length()) {
        currentChar = '\0';  // 到达文件尾
    } else {
        currentChar = sourceCode[position];
        
        // 处理换行
        if (currentChar == '\n') {
            line++;
            column = 1;
        }
    }
}

void Lexer::skipWhitespace() {
    // 跳过所有空白字符（空格、制表符和换行符）
    while (currentChar != '\0' && isspace(currentChar)) {
        advance();
    }
}

void Lexer::skipComment() {
    // 跳过注释 {这是注释}
    if (currentChar == '{') {
        advance();  // 跳过'{'
        
        // 一直读取直到遇到'}'或文件结束
        while (currentChar != '\0' && currentChar != '}') {
            advance();
        }
        
        if (currentChar == '}') {
            advance();  // 跳过'}'
        }
    }
}

char Lexer::peek() {
    // 查看下一个字符（不前进）
    if (position + 1 >= sourceCode.length()) {
        return '\0';
    }
    return sourceCode[position + 1];
}

Token Lexer::number() {
    // 识别数字（无符号整数）
    std::string result;
    int startColumn = column;
    
    while (currentChar != '\0' && isdigit(currentChar)) {
        result += currentChar;
        advance();
    }
    
    return Token(NUMBER, result, line, startColumn);
}

Token Lexer::identifier() {
    // 识别标识符或关键字
    std::string result;
    int startColumn = column;
    
    // 标识符的第一个字符必须是字母
    if (isalpha(currentChar)) {
        result += currentChar;
        advance();
        
        // 标识符的后续字符可以是字母或数字
        while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
            result += currentChar;
            advance();
        }
        
        // 检查是否是关键字
        std::string upperResult = result;
        // 转换为大写以便与关键字表比较
        for (char& c : upperResult) {
            c = toupper(c);
        }
        
        if (keywords.find(upperResult) != keywords.end()) {
            // 是关键字
            return Token(keywords[upperResult], result, line, startColumn);
        } else {
            // 是标识符
            // 添加到符号表
            symbolTable.addSymbol(result);
            return Token(IDENTIFIER, result, line, startColumn);
        }
    }
    
    // 不应该到达这里
    return Token(IDENTIFIER, "", line, startColumn);
}

Token Lexer::getNextToken() {
    // 获取下一个Token
    
    // 跳过空白字符和注释
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            skipWhitespace();
            continue;
        }
        
        if (currentChar == '{') {
            skipComment();
            continue;
        }
        
        break;
    }
    
    // 如果到达文件尾，返回特殊的EOF Token
    if (currentChar == '\0') {
        return Token(HASH, "#", line, column);
    }
    
    // 识别数字
    if (isdigit(currentChar)) {
        return number();
    }
    
    // 识别标识符或关键字
    if (isalpha(currentChar)) {
        return identifier();
    }
    
    // 识别运算符和分隔符
    int startColumn = column;
    char ch = currentChar;
    
    switch (ch) {
        case '+':
            advance();
            return Token(PLUS, "+", line, startColumn);
        case '-':
            advance();
            return Token(MINUS, "-", line, startColumn);
        case '*':
            advance();
            return Token(MULTIPLY, "*", line, startColumn);
        case '/':
            advance();
            return Token(DIVIDE, "/", line, startColumn);
        case '=':
            advance();
            return Token(EQUAL, "=", line, startColumn);
        case '<':
            advance();
            if (currentChar == '=') {
                advance();
                return Token(LESS_EQUAL, "<=", line, startColumn);
            } else if (currentChar == '>') {
                advance();
                return Token(NOT_EQUAL, "<>", line, startColumn);
            }
            return Token(LESS, "<", line, startColumn);
        case '>':
            advance();
            if (currentChar == '=') {
                advance();
                return Token(GREATER_EQUAL, ">=", line, startColumn);
            }
            return Token(GREATER, ">", line, startColumn);
        case '.':
            advance();
            return Token(DOT, ".", line, startColumn);
        case ',':
            advance();
            return Token(COMMA, ",", line, startColumn);
        case ';':
            advance();
            return Token(SEMICOLON, ";", line, startColumn);
        case ':':
            advance();
            if (currentChar == '=') {
                advance();
                return Token(ASSIGN, ":=", line, startColumn);
            }
            return Token(COLON, ":", line, startColumn);
        case '(':
            advance();
            return Token(LEFT_PAREN, "(", line, startColumn);
        case ')':
            advance();
            return Token(RIGHT_PAREN, ")", line, startColumn);
        default:
            // 未知字符，报错或跳过
            std::cerr << "错误：未知字符 '" << ch << "' 在第 " << line << " 行，第 " << column << " 列" << std::endl;
            advance();
            return getNextToken();  // 递归调用以获取下一个有效的Token
    }
}

std::vector<Token> Lexer::getAllTokens() {
    std::vector<Token> tokens;
    
    // 重置位置
    position = 0;
    line = 1;
    column = 1;
    if (!sourceCode.empty()) {
        currentChar = sourceCode[0];
    } else {
        currentChar = '\0';
    }
    
    // 获取所有Token
    Token token = getNextToken();
    while (token.getType() != HASH) {  // 直到文件尾
        tokens.push_back(token);
        token = getNextToken();
    }
    
    // 添加EOF Token
    tokens.push_back(token);
    
    return tokens;
}

SymbolTable Lexer::getSymbolTable() const {
    return symbolTable;
} 