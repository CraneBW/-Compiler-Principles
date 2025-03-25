#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <fstream>

// Token结构
struct Token {
    int code;           // 内码
    std::string value;  // 词素
    int lineNo;         // 行号
    int columnNo;       // 列号
};

class Lexer {
private:
    std::ifstream input;
    std::string currentLine;
    int lineNo;
    int columnNo;
    char currentChar;
    
    void advance();
    void skipWhitespace();
    void skipComment();
    Token getNumber();
    Token getIdentifier();
    Token getOperator();
    Token getDelimiter();

public:
    Lexer(const std::string& filename);
    ~Lexer();
    
    // 获取下一个token
    Token nextToken();
    
    // 获取当前行号和列号
    int getCurrentLineNo() const { return lineNo; }
    int getCurrentColumnNo() const { return columnNo; }
    
    // 重置词法分析器
    void reset();
};

#endif // LEXER_H 