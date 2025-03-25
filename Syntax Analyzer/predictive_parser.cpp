#include "predictive_parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

PredictiveParser::PredictiveParser() : lineNo(1), columnNo(0), errorRecoveryMode(false), errorCount(0) {
    // 不再尝试打开文件，词法分析器将在parse方法中初始化
    initializeParseTable();
    initializeRules();
    initializeSyncSets();
    initializeFollowSets();
    enterScope("global");  // 进入全局作用域
}

void PredictiveParser::error(const std::string& message) {
    std::cerr << "Error at line " << lineNo << ", column " << columnNo << ": " << message << std::endl;
    errorCount++;
    errorRecoveryMode = true;
}

Token PredictiveParser::nextToken() {
    if (!lexer) {
        throw std::runtime_error("词法分析器未初始化");
    }
    Token token = lexer->nextToken();
    lineNo = lexer->getCurrentLineNo();
    columnNo = lexer->getCurrentColumnNo();
    return token;
}

bool PredictiveParser::isTerminal(int token) {
    return token >= 1 && token <= 35;
}

bool PredictiveParser::isNonTerminal(int token) {
    return token >= 128 && token <= 161;
}

void PredictiveParser::pushRule(int ruleIndex) {
    // 从右到左压入产生式右部的符号
    for (int i = rules[ruleIndex].size() - 1; i >= 0; --i) {
        if (rules[ruleIndex][i] != 0) {
            parseStack.push(rules[ruleIndex][i]);
        }
    }
}

void PredictiveParser::addToSymbolTable(const std::string& id, const std::string& type,
                                      const std::string& value, const std::string& scope) {
    SymbolTableEntry entry;
    entry.type = type;
    entry.value = value;
    entry.scope = scope;
    symbolTable[id].push_back(entry);
}

void PredictiveParser::initializeRules() {
    // 初始化产生式规则
    rules = {
        {129, 130, 26, 0},    // <程序> → <程序首部><分程序>.
        {1, 34, 28, 0},       // <程序首部> → PROGRAM标识符；
        {131, 134, 138, 150, 0}, // <分程序> → <常量说明部分><变量说明部分><过程说明部分><复合语句>
        {2, 132, 133, 28, 0}, // <常量说明部分> → CONST<常量定义><常量定义后缀>；
        {0},                  // ε
        {34, 20, 33, 0},     // <常量定义> → 标识符=无符号整数
        {27, 132, 133, 0},   // <常量定义后缀> → ，<常量定义><常量定义后缀>
        {3, 135, 136, 0},    // <变量说明部分> → VAR<变量定义><变量定义后缀>；
        {34, 147, 29, 137, 28, 0}, // <变量定义> → 标识符<标识符后缀>：<类型>；
        {27, 34, 147, 0},    // <标识符后缀> → ，标识符<标识符后缀>
        {135, 136, 0},       // <变量定义后缀> → <变量定义><变量定义后缀>
        {40, 0},             // <类型> → INTEGER
        {50, 0},             // <类型> → LONG
        {139, 130, 28, 140, 0}, // <过程说明部分> → <过程首部><分程序>；<过程说明部分后缀>
        {6, 34, 158, 28, 0}, // <过程首部> → PROCEDURE标识符<参数部分>；
        {31, 34, 29, 137, 32, 0}, // <参数部分> → (标识符：<类型>)
        {0},                  // ε
        {142, 0},            // <语句> → <赋值或调用语句>
        {144, 0},            // <语句> → <条件语句>
        {145, 0},            // <语句> → <当型循环语句>
        {146, 0},            // <语句> → <读语句>
        {148, 0},            // <语句> → <写语句>
        {150, 0},            // <语句> → <复合语句>
        {0},                  // ε
        {34, 143, 0},        // <赋值或调用语句> → 标识符<后缀>
        {30, 153, 0},        // <后缀> → :=<表达式>
        {31, 153, 32, 0},    // <后缀> → (<表达式>)
        {0},                  // ε
        {7, 152, 8, 141, 0}, // <条件语句> → IF<条件>THEN<语句>
        {9, 152, 10, 141, 0}, // <当型循环语句> → WHILE<条件>DO<语句>
        {11, 31, 34, 147, 32, 0}, // <读语句> → READ(标识符<标识符后缀>)
        {12, 31, 153, 149, 32, 0}, // <写语句> → WRITE(<表达式><表达式后缀>)
        {13, 141, 151, 14, 0}, // <复合语句> → BEGIN<语句><语句后缀>END
        {28, 141, 151, 0},    // <语句后缀> → ；<语句><语句后缀>
        {0},                  // ε
        {153, 161, 153, 0},  // <条件> → <表达式><关系运算符><表达式>
        {15, 153, 0},        // <条件> → ODD<表达式>
        {16, 155, 154, 0},   // <表达式> → +<项><项后缀>
        {17, 155, 154, 0},   // <表达式> → -<项><项后缀>
        {155, 154, 0},       // <表达式> → <项><项后缀>
        {16, 155, 154, 0},   // <项后缀> → <加型运算符><项><项后缀>
        {17, 155, 154, 0},   // <项后缀> → <加型运算符><项><项后缀>
        {0},                  // ε
        {157, 156, 0},       // <项> → <因子><因子后缀>
        {18, 157, 156, 0},   // <因子后缀> → <乘型运算符><因子><因子后缀>
        {19, 157, 156, 0},   // <因子后缀> → <乘型运算符><因子><因子后缀>
        {0},                  // ε
        {34, 0},             // <因子> → 标识符
        {33, 0},             // <因子> → 无符号整数
        {31, 153, 32, 0},    // <因子> → (<表达式>)
        {16, 0},             // <加型运算符> → +
        {17, 0},             // <加型运算符> → -
        {18, 0},             // <乘型运算符> → *
        {19, 0},             // <乘型运算符> → /
        {20, 0},             // <关系运算符> → =
        {21, 0},             // <关系运算符> → <>
        {22, 0},             // <关系运算符> → <
        {23, 0},             // <关系运算符> → <=
        {24, 0},             // <关系运算符> → >
        {25, 0}              // <关系运算符> → >=
    };
}

void PredictiveParser::initializeParseTable() {
    // 初始化预测分析表
    // 行：非终结符（128-161）
    // 列：终结符（1-35）
    // 值：产生式编号（-1表示错误）
    parseTable.resize(34, std::vector<int>(35, -1));
    
    // <程序>的预测分析表项
    parseTable[0][1] = 0;  // 当看到PROGRAM时，使用第0号产生式
    
    // <程序首部>的预测分析表项
    parseTable[1][1] = 1;  // 当看到PROGRAM时，使用第1号产生式
    
    // <分程序>的预测分析表项
    parseTable[2][2] = 3;  // 当看到CONST时，使用第3号产生式
    parseTable[2][3] = 3;  // 当看到VAR时，使用第3号产生式
    parseTable[2][6] = 3;  // 当看到PROCEDURE时，使用第3号产生式
    parseTable[2][13] = 3; // 当看到BEGIN时，使用第3号产生式
    
    // <常量说明部分>的预测分析表项
    parseTable[3][2] = 3;  // 当看到CONST时，使用第3号产生式
    parseTable[3][3] = 4;  // 当看到VAR时，使用第4号产生式
    parseTable[3][6] = 4;  // 当看到PROCEDURE时，使用第4号产生式
    parseTable[3][13] = 4; // 当看到BEGIN时，使用第4号产生式
    
    // <常量定义>的预测分析表项
    parseTable[4][34] = 5;  // 当看到标识符时，使用第5号产生式
    
    // <常量定义后缀>的预测分析表项
    parseTable[5][27] = 6;  // 当看到','时，使用第6号产生式
    parseTable[5][3] = 7;   // 当看到VAR时，使用第7号产生式
    parseTable[5][6] = 7;   // 当看到PROCEDURE时，使用第7号产生式
    parseTable[5][13] = 7;  // 当看到BEGIN时，使用第7号产生式
    
    // <变量说明部分>的预测分析表项
    parseTable[6][3] = 7;   // 当看到VAR时，使用第7号产生式
    parseTable[6][6] = 8;   // 当看到PROCEDURE时，使用第8号产生式
    parseTable[6][13] = 8;  // 当看到BEGIN时，使用第8号产生式
    
    // <变量定义>的预测分析表项
    parseTable[7][34] = 8;  // 当看到标识符时，使用第8号产生式
    
    // <标识符后缀>的预测分析表项
    parseTable[8][27] = 9;  // 当看到','时，使用第9号产生式
    parseTable[8][29] = 10; // 当看到':'时，使用第10号产生式
    
    // <变量定义后缀>的预测分析表项
    parseTable[9][34] = 11; // 当看到标识符时，使用第11号产生式
    parseTable[9][6] = 12;  // 当看到PROCEDURE时，使用第12号产生式
    parseTable[9][13] = 12; // 当看到BEGIN时，使用第12号产生式
    
    // <类型>的预测分析表项
    parseTable[10][40] = 12; // 当看到INTEGER时，使用第12号产生式
    parseTable[10][50] = 13; // 当看到LONG时，使用第13号产生式
    
    // <过程说明部分>的预测分析表项
    parseTable[11][6] = 13;  // 当看到PROCEDURE时，使用第13号产生式
    parseTable[11][13] = 14; // 当看到BEGIN时，使用第14号产生式
    
    // <过程首部>的预测分析表项
    parseTable[12][6] = 14;  // 当看到PROCEDURE时，使用第14号产生式
    
    // <参数部分>的预测分析表项
    parseTable[13][31] = 15; // 当看到'('时，使用第15号产生式
    parseTable[13][28] = 16; // 当看到';'时，使用第16号产生式
    
    // <过程说明部分后缀>的预测分析表项
    parseTable[14][6] = 17;  // 当看到PROCEDURE时，使用第17号产生式
    parseTable[14][13] = 18; // 当看到BEGIN时，使用第18号产生式
    
    // <语句>的预测分析表项
    parseTable[15][34] = 19;  // 当看到标识符时，使用第19号产生式
    parseTable[15][7] = 20;   // 当看到IF时，使用第20号产生式
    parseTable[15][9] = 21;   // 当看到WHILE时，使用第21号产生式
    parseTable[15][11] = 22;  // 当看到READ时，使用第22号产生式
    parseTable[15][12] = 23;  // 当看到WRITE时，使用第23号产生式
    parseTable[15][13] = 24;  // 当看到BEGIN时，使用第24号产生式
    
    // <赋值或调用语句>的预测分析表项
    parseTable[16][34] = 25;  // 当看到标识符时，使用第25号产生式
    
    // <后缀>的预测分析表项
    parseTable[17][30] = 26;  // 当看到':='时，使用第26号产生式
    parseTable[17][31] = 27;  // 当看到'('时，使用第27号产生式
    
    // <条件语句>的预测分析表项
    parseTable[18][7] = 28;   // 当看到IF时，使用第28号产生式
    
    // <当型循环语句>的预测分析表项
    parseTable[19][9] = 29;   // 当看到WHILE时，使用第29号产生式
    
    // <读语句>的预测分析表项
    parseTable[20][11] = 30;  // 当看到READ时，使用第30号产生式
    
    // <写语句>的预测分析表项
    parseTable[21][12] = 31;  // 当看到WRITE时，使用第31号产生式
    
    // <复合语句>的预测分析表项
    parseTable[22][13] = 32;  // 当看到BEGIN时，使用第32号产生式
    
    // <语句后缀>的预测分析表项
    parseTable[23][28] = 33;  // 当看到';'时，使用第33号产生式
    parseTable[23][14] = 34;  // 当看到END时，使用第34号产生式
    
    // <条件>的预测分析表项
    parseTable[24][34] = 35;  // 当看到标识符时，使用第35号产生式
    parseTable[24][33] = 35;  // 当看到无符号整数时，使用第35号产生式
    parseTable[24][31] = 35;  // 当看到'('时，使用第35号产生式
    parseTable[24][15] = 36;  // 当看到ODD时，使用第36号产生式
    
    // <表达式>的预测分析表项
    parseTable[25][16] = 37;  // 当看到'+'时，使用第37号产生式
    parseTable[25][17] = 38;  // 当看到'-'时，使用第38号产生式
    parseTable[25][34] = 39;  // 当看到标识符时，使用第39号产生式
    parseTable[25][33] = 39;  // 当看到无符号整数时，使用第39号产生式
    parseTable[25][31] = 39;  // 当看到'('时，使用第39号产生式
    
    // <项后缀>的预测分析表项
    parseTable[26][16] = 40;  // 当看到'+'时，使用第40号产生式
    parseTable[26][17] = 41;  // 当看到'-'时，使用第41号产生式
    parseTable[26][28] = 42;  // 当看到';'时，使用第42号产生式
    parseTable[26][14] = 42;  // 当看到END时，使用第42号产生式
    parseTable[26][32] = 42;  // 当看到')'时，使用第42号产生式
    parseTable[26][20] = 42;  // 当看到'='时，使用第42号产生式
    parseTable[26][21] = 42;  // 当看到'<>'时，使用第42号产生式
    parseTable[26][22] = 42;  // 当看到'<'时，使用第42号产生式
    parseTable[26][23] = 42;  // 当看到'<='时，使用第42号产生式
    parseTable[26][24] = 42;  // 当看到'>'时，使用第42号产生式
    parseTable[26][25] = 42;  // 当看到'>='时，使用第42号产生式
    
    // <项>的预测分析表项
    parseTable[27][34] = 43;  // 当看到标识符时，使用第43号产生式
    parseTable[27][33] = 43;  // 当看到无符号整数时，使用第43号产生式
    parseTable[27][31] = 43;  // 当看到'('时，使用第43号产生式
    
    // <因子后缀>的预测分析表项
    parseTable[28][18] = 44;  // 当看到'*'时，使用第44号产生式
    parseTable[28][19] = 45;  // 当看到'/'时，使用第45号产生式
    parseTable[28][16] = 46;  // 当看到'+'时，使用第46号产生式
    parseTable[28][17] = 46;  // 当看到'-'时，使用第46号产生式
    parseTable[28][28] = 46;  // 当看到';'时，使用第46号产生式
    parseTable[28][14] = 46;  // 当看到END时，使用第46号产生式
    parseTable[28][32] = 46;  // 当看到')'时，使用第46号产生式
    parseTable[28][20] = 46;  // 当看到'='时，使用第46号产生式
    parseTable[28][21] = 46;  // 当看到'<>'时，使用第46号产生式
    parseTable[28][22] = 46;  // 当看到'<'时，使用第46号产生式
    parseTable[28][23] = 46;  // 当看到'<='时，使用第46号产生式
    parseTable[28][24] = 46;  // 当看到'>'时，使用第46号产生式
    parseTable[28][25] = 46;  // 当看到'>='时，使用第46号产生式
    
    // <因子>的预测分析表项
    parseTable[29][34] = 47;  // 当看到标识符时，使用第47号产生式
    parseTable[29][33] = 48;  // 当看到无符号整数时，使用第48号产生式
    parseTable[29][31] = 49;  // 当看到'('时，使用第49号产生式
    
    // <加型运算符>的预测分析表项
    parseTable[30][16] = 50;  // 当看到'+'时，使用第50号产生式
    parseTable[30][17] = 51;  // 当看到'-'时，使用第51号产生式
    
    // <乘型运算符>的预测分析表项
    parseTable[31][18] = 52;  // 当看到'*'时，使用第52号产生式
    parseTable[31][19] = 53;  // 当看到'/'时，使用第53号产生式
    
    // <关系运算符>的预测分析表项
    parseTable[32][20] = 54;  // 当看到'='时，使用第54号产生式
    parseTable[32][21] = 55;  // 当看到'<>'时，使用第55号产生式
    parseTable[32][22] = 56;  // 当看到'<'时，使用第56号产生式
    parseTable[32][23] = 57;  // 当看到'<='时，使用第57号产生式
    parseTable[32][24] = 58;  // 当看到'>'时，使用第58号产生式
    parseTable[32][25] = 59;  // 当看到'>='时，使用第59号产生式
}

bool PredictiveParser::parse(const std::string& input) {
    // 将输入内容写入临时文件
    std::ofstream tempFile("temp_predictive_test.txt");
    tempFile << input;
    tempFile.close();
    
    // 初始化词法分析器
    lexer = std::make_unique<Lexer>("temp_predictive_test.txt");
    
    parseStack = std::stack<int>();  // 清空栈
    parseStack.push(128);  // 压入<程序>的编号
    lookahead = nextToken();

    while (!parseStack.empty()) {
        int top = parseStack.top();
        parseStack.pop();

        if (top < 128) {  // 终结符
            if (top == lookahead.code) {
                lookahead = nextToken();
            } else {
                error("语法错误：期望token: " + std::to_string(top) + 
                      ", 实际token: " + std::to_string(lookahead.code));
                return false;
            }
        } else {  // 非终结符
            int ruleIndex = parseTable[top - 128][lookahead.code];
            if (ruleIndex == -1) {
                error("语法错误：非预期的token " + std::to_string(lookahead.code));
                if (errorRecoveryMode) {
                    synchronize();
                    continue;
                }
                return false;
            }
            pushRule(ruleIndex);
        }
    }

    return true;
}

void PredictiveParser::printSymbolTable() const {
    std::cout << "符号表内容：" << std::endl;
    for (const auto& entry : symbolTable) {
        for (const auto& symbol : entry.second) {
            std::cout << "[" << entry.first << "] type: " << symbol.type;
            if (!symbol.value.empty()) {
                std::cout << ", value: " << symbol.value;
            }
            std::cout << ", scope: " << symbol.scope << std::endl;
        }
    }
}

// 符号表管理方法实现
void PredictiveParser::enterScope(const std::string& scopeName) {
    scopeStack.push(scopeName);
    currentLevel++;
}

void PredictiveParser::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop();
        currentLevel--;
    }
}

bool PredictiveParser::declareSymbol(const std::string& id, const std::string& type,
                                   const std::string& value, bool isParam) {
    // 检查重复声明
    checkDuplicateDeclaration(id);
    
    // 创建新的符号表项
    SymbolTableEntry entry;
    entry.type = type;
    entry.value = value;
    entry.scope = getCurrentScope();
    entry.level = currentLevel;
    entry.isParameter = isParam;
    
    // 添加到符号表
    symbolTable[id].push_back(entry);
    return true;
}

SymbolTableEntry* PredictiveParser::lookupSymbol(const std::string& id) {
    auto it = symbolTable.find(id);
    if (it != symbolTable.end() && !it->second.empty()) {
        return &(it->second.back());
    }
    return nullptr;
}

bool PredictiveParser::checkType(const std::string& id, const std::string& expectedType) {
    SymbolTableEntry* entry = lookupSymbol(id);
    if (!entry) {
        error("未声明的标识符: " + id);
        return false;
    }
    
    if (entry->type != expectedType) {
        error("类型不匹配: " + id + " 期望类型 " + expectedType + " 实际类型 " + entry->type);
        return false;
    }
    
    return true;
}

void PredictiveParser::checkDuplicateDeclaration(const std::string& id) {
    SymbolTableEntry* entry = lookupSymbol(id);
    if (entry && entry->level == currentLevel) {
        error("重复声明: " + id);
    }
}

std::string PredictiveParser::getCurrentScope() const {
    return scopeStack.empty() ? "global" : scopeStack.top();
}

const std::map<std::string, std::vector<SymbolTableEntry>>& PredictiveParser::getSymbolTable() const {
    return symbolTable;
}

void PredictiveParser::initializeSyncSets() {
    syncSets.resize(34);  // 非终结符数量
    
    // 为每个非终结符设置同步集
    // <程序>的同步集
    syncSets[0].insert({1, 2, 3});  // CONST, VAR, PROCEDURE
    
    // <常量说明>的同步集
    syncSets[1].insert({2, 3, 4});  // VAR, PROCEDURE, BEGIN
    
    // <变量说明>的同步集
    syncSets[2].insert({3, 4});  // PROCEDURE, BEGIN
    
    // <过程说明>的同步集
    syncSets[3].insert({4});  // BEGIN
    
    // <语句>的同步集
    syncSets[4].insert({13, 14, 15, 16, 17});  // END, ELSE, THEN, DO, ;
}

void PredictiveParser::initializeFollowSets() {
    followSets.resize(34);  // 非终结符数量
    
    // 为每个非终结符设置FOLLOW集
    // FOLLOW(<程序>) = {EOF}
    followSets[0].insert(0);
    
    // FOLLOW(<常量说明>) = {VAR, PROCEDURE, BEGIN}
    followSets[1].insert({2, 3, 4});
    
    // FOLLOW(<变量说明>) = {PROCEDURE, BEGIN}
    followSets[2].insert({3, 4});
    
    // FOLLOW(<过程说明>) = {BEGIN}
    followSets[3].insert({4});
    
    // FOLLOW(<语句>) = {END, ELSE, ;, EOF}
    followSets[4].insert({13, 14, 17, 0});
}

bool PredictiveParser::isInSyncSet(int nonTerminal, int token) {
    if (nonTerminal < 128 || nonTerminal >= 162) return false;
    return syncSets[nonTerminal - 128].count(token) > 0;
}

bool PredictiveParser::isInFollowSet(int nonTerminal, int token) {
    if (nonTerminal < 128 || nonTerminal >= 162) return false;
    return followSets[nonTerminal - 128].count(token) > 0;
}

void PredictiveParser::synchronize() {
    while (!parseStack.empty()) {
        int top = parseStack.top();
        if (top >= 128) {  // 非终结符
            if (isInSyncSet(top, lookahead.code)) {
                return;  // 找到同步点
            }
            if (isInFollowSet(top, lookahead.code)) {
                parseStack.pop();
                return;  // 在FOLLOW集中找到同步点
            }
            parseStack.pop();
        } else {  // 终结符
            if (top == lookahead.code) {
                parseStack.pop();
                lookahead = nextToken();
                return;  // 找到匹配的终结符
            }
            parseStack.pop();
        }
    }
    // 如果栈空了还没找到同步点，就继续读取输入
    lookahead = nextToken();
} 