#ifndef PREDICTIVE_PARSER_H
#define PREDICTIVE_PARSER_H

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <memory>  // 用于std::unique_ptr
#include <set>
#include "lexer.h"
#include "symbol_table.h"

class PredictiveParser {
private:
    std::stack<int> parseStack;  // 改为存储整数
    std::map<std::string, std::vector<SymbolTableEntry>> symbolTable;
    std::vector<std::vector<int>> parseTable;
    std::vector<std::vector<int>> rules;
    std::vector<std::set<int>> syncSets;
    std::vector<std::set<int>> followSets;
    Token lookahead;
    std::unique_ptr<Lexer> lexer;
    int lineNo;
    int columnNo;
    bool errorRecoveryMode;
    int errorCount;
    
    // 符号表管理相关
    std::stack<std::string> scopeStack;  // 作用域栈
    int currentLevel;                    // 当前嵌套层次
    
    // 辅助函数
    void error(const std::string& message);
    Token nextToken();
    bool isTerminal(int token);
    bool isNonTerminal(int token);
    void pushRule(int ruleIndex);
    
    // 符号表管理方法
    void enterScope(const std::string& scopeName);
    void exitScope();
    bool declareSymbol(const std::string& id, const std::string& type, 
                      const std::string& value = "", bool isParam = false);
    SymbolTableEntry* lookupSymbol(const std::string& id);
    bool checkType(const std::string& id, const std::string& expectedType);
    void checkDuplicateDeclaration(const std::string& id);

    void initializeSyncSets();
    void initializeFollowSets();
    bool isInSyncSet(int nonTerminal, int token);
    bool isInFollowSet(int nonTerminal, int token);
    void synchronize();
    void addToSymbolTable(const std::string& id, const std::string& type,
                         const std::string& value, const std::string& scope);

public:
    PredictiveParser();
    bool parse(const std::string& input);
    void printSymbolTable() const;
    
    // 初始化预测分析表和产生式规则
    void initializeParseTable();
    void initializeRules();
    
    // 获取当前作用域
    std::string getCurrentScope() const;
    
    // 获取符号表
    const std::map<std::string, std::vector<SymbolTableEntry>>& getSymbolTable() const;
};

#endif // PREDICTIVE_PARSER_H 