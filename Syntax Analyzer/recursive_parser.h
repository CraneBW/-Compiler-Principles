#ifndef RECURSIVE_PARSER_H
#define RECURSIVE_PARSER_H

#include <string>
#include <map>
#include <vector>
#include <memory>  // 用于std::unique_ptr
#include "lexer.h"
#include "symbol_table.h"



class RecursiveParser {
private:
    Token lookahead;
    std::map<std::string, std::vector<SymbolTableEntry>> symbolTable;
    int lineNo;            // 当前行号
    int columnNo;          // 当前列号
    std::unique_ptr<Lexer> lexer;  // 添加词法分析器成员变量
    
    // 辅助函数
    void error(const std::string& message);
    void match(int expected);
    Token nextToken();
    
    // 非终结符对应的递归函数
    void program();
    void proghead();
    void block();
    void consexpl();
    void consdefi();
    void conssuff();
    void varexl();
    void vardefi();
    void varsuff();
    void procdefi();
    void procedh();
    void procsuff();
    void argument();
    void sentence();
    void assipro();
    void suffix();
    void ifsent();
    void whilsent();
    void read();
    void write();
    void compsent();
    void sentsuff();
    void condition();
    void express();
    void termsuff();
    void term();
    void factsuff();
    void factor();
    void addoper();
    void muloper();
    void respoper();
    void idsuff();
    void exprsuff();
    void typeil();

public:
    RecursiveParser();
    bool parse(const std::string& input);
    void printSymbolTable() const;
};

#endif // RECURSIVE_PARSER_H 