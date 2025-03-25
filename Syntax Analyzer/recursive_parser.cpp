#include "recursive_parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

RecursiveParser::RecursiveParser() : lineNo(1), columnNo(0) {
    // 不再尝试打开文件，词法分析器将在parse方法中初始化
}

void RecursiveParser::error(const std::string& message) {
    std::cout << "错误在第 " << lineNo << " 行，第 " << columnNo << " 列: " << message << std::endl;
    throw std::runtime_error(message);
}

void RecursiveParser::match(int expected) {
    if (lookahead.code == expected) {
        lookahead = nextToken();
    } else {
        error("期望的token: " + std::to_string(expected) + 
              ", 实际token: " + std::to_string(lookahead.code));
    }
}

Token RecursiveParser::nextToken() {
    Token token = lexer->nextToken();
    lineNo = token.lineNo;
    columnNo = token.columnNo;
    return token;
}

bool RecursiveParser::parse(const std::string& input) {
    // 将输入内容写入临时文件
    std::ofstream tempFile("temp_recursive_test.txt");
    tempFile << input;
    tempFile.close();
    
    // 初始化词法分析器
    lexer = std::make_unique<Lexer>("temp_recursive_test.txt");
    
    lookahead = nextToken();
    try {
        program();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "解析错误: " << e.what() << std::endl;
        return false;
    }
}

void RecursiveParser::program() {
    proghead();
    block();
    match(26); // 匹配 '.'
}

void RecursiveParser::proghead() {
    match(1);  // PROGRAM
    // 获取标识符并添加到符号表
    std::string id = lookahead.value;
    match(34); // 标识符
    SymbolTableEntry entry;
    entry.type = "PROGRAM";
    entry.scope = "global";
    entry.level = 0;
    entry.isParameter = false;
    symbolTable[id].push_back(entry);
    match(28); // 匹配 ';'
}

void RecursiveParser::block() {
    if (lookahead.code == 2) { // CONST
        consexpl();
    }
    if (lookahead.code == 3) { // VAR
        varexl();
    }
    if (lookahead.code == 6) { // PROCEDURE
        procdefi();
    }
    compsent();
}

void RecursiveParser::consexpl() {
    match(2);  // CONST
    consdefi();
    conssuff();
}

void RecursiveParser::consdefi() {
    std::string id = lookahead.value;
    match(34); // 标识符
    match(20); // '='
    std::string value = lookahead.value;
    match(33); // 无符号整数
    
    SymbolTableEntry entry;
    entry.type = "CONST";
    entry.value = value;
    entry.scope = "global";
    entry.level = 0;
    entry.isParameter = false;
    symbolTable[id].push_back(entry);
}

void RecursiveParser::conssuff() {
    if (lookahead.code == 27) { // ','
        match(27);
        consdefi();
        conssuff();
    }
}

void RecursiveParser::varexl() {
    match(3);  // VAR
    vardefi();
    varsuff();
}

void RecursiveParser::vardefi() {
    std::string id = lookahead.value;
    match(34); // 标识符
    idsuff();
    match(29); // ':'
    std::string type = lookahead.value;
    typeil();
    match(28); // ';'
    
    SymbolTableEntry entry;
    entry.type = "VAR";
    entry.value = "";
    entry.scope = "global";
    entry.level = 0;
    entry.isParameter = false;
    symbolTable[id].push_back(entry);
}

void RecursiveParser::varsuff() {
    if (lookahead.code == 34) { // 标识符
        vardefi();
        varsuff();
    }
}

void RecursiveParser::procdefi() {
    procedh();
    block();
    match(28); // ';'
    procsuff();
}

void RecursiveParser::procedh() {
    match(6);  // PROCEDURE
    std::string id = lookahead.value;
    match(34); // 标识符
    
    SymbolTableEntry entry;
    entry.type = "PROCEDURE";
    entry.scope = "global";
    entry.level = 0;
    entry.isParameter = false;
    symbolTable[id].push_back(entry);
    
    argument();
    match(28); // ';'
}

void RecursiveParser::procsuff() {
    if (lookahead.code == 6) { // PROCEDURE
        procdefi();
    }
}

void RecursiveParser::argument() {
    if (lookahead.code == 31) { // '('
        match(31);
        std::string id = lookahead.value;
        match(34); // 标识符
        match(29); // ':'
        std::string type = lookahead.value;
        typeil();
        match(32); // ')'
        
        SymbolTableEntry entry;
        entry.type = type;
        entry.scope = "local";
        entry.level = 1;
        entry.isParameter = true;
        symbolTable[id].push_back(entry);
    }
}

void RecursiveParser::sentence() {
    switch (lookahead.code) {
        case 34: // 标识符
            assipro();
            break;
        case 7:  // IF
            ifsent();
            break;
        case 9:  // WHILE
            whilsent();
            break;
        case 11: // READ
            read();
            break;
        case 12: // WRITE
            write();
            break;
        case 13: // BEGIN
            compsent();
            break;
    }
}

void RecursiveParser::assipro() {
    std::string id = lookahead.value;
    match(34); // 标识符
    suffix();
}

void RecursiveParser::suffix() {
    if (lookahead.code == 30) { // ':='
        match(30);
        express();
    } else if (lookahead.code == 31) { // '('
        match(31);
        express();
        match(32); // ')'
    }
}

void RecursiveParser::ifsent() {
    match(7);  // IF
    condition();
    match(8);  // THEN
    sentence();
}

void RecursiveParser::whilsent() {
    match(9);  // WHILE
    condition();
    match(10); // DO
    sentence();
}

void RecursiveParser::read() {
    match(11); // READ
    match(31); // '('
    std::string id = lookahead.value;
    match(34); // 标识符
    idsuff();
    match(32); // ')'
}

void RecursiveParser::write() {
    match(12); // WRITE
    match(31); // '('
    express();
    exprsuff();
    match(32); // ')'
}

void RecursiveParser::compsent() {
    match(13); // BEGIN
    sentence();
    sentsuff();
    match(14); // END
}

void RecursiveParser::sentsuff() {
    if (lookahead.code == 28) { // ';'
        match(28);
        sentence();
        sentsuff();
    }
}

void RecursiveParser::condition() {
    if (lookahead.code == 15) { // ODD
        match(15);
        express();
    } else {
        express();
        respoper();
        express();
    }
}

void RecursiveParser::express() {
    if (lookahead.code == 16) { // '+'
        match(16);
        term();
        termsuff();
    } else if (lookahead.code == 17) { // '-'
        match(17);
        term();
        termsuff();
    } else {
        term();
        termsuff();
    }
}

void RecursiveParser::termsuff() {
    if (lookahead.code == 16 || lookahead.code == 17) { // '+' 或 '-'
        addoper();
        term();
        termsuff();
    }
}

void RecursiveParser::term() {
    factor();
    factsuff();
}

void RecursiveParser::factsuff() {
    if (lookahead.code == 18 || lookahead.code == 19) { // '*' 或 '/'
        muloper();
        factor();
        factsuff();
    }
}

void RecursiveParser::factor() {
    if (lookahead.code == 34) { // 标识符
        match(34);
    } else if (lookahead.code == 33) { // 无符号整数
        match(33);
    } else if (lookahead.code == 31) { // '('
        match(31);
        express();
        match(32); // ')'
    }
}

void RecursiveParser::addoper() {
    if (lookahead.code == 16) { // '+'
        match(16);
    } else if (lookahead.code == 17) { // '-'
        match(17);
    }
}

void RecursiveParser::muloper() {
    if (lookahead.code == 18) { // '*'
        match(18);
    } else if (lookahead.code == 19) { // '/'
        match(19);
    }
}

void RecursiveParser::respoper() {
    switch (lookahead.code) {
        case 20: // '='
        case 21: // '<>'
        case 22: // '<'
        case 23: // '<='
        case 24: // '>'
        case 25: // '>='
            match(lookahead.code);
            break;
        default:
            error("非预期的关系运算符");
    }
}

void RecursiveParser::idsuff() {
    if (lookahead.code == 27) { // ','
        match(27);
        match(34); // 标识符
        idsuff();
    }
}

void RecursiveParser::exprsuff() {
    if (lookahead.code == 27) { // ','
        match(27);
        express();
        exprsuff();
    }
}

void RecursiveParser::typeil() {
    if (lookahead.code == 40) { // INTEGER
        match(40);
    } else if (lookahead.code == 50) { // LONG
        match(50);
    } else {
        error("非预期的类型");
    }
}

void RecursiveParser::printSymbolTable() const {
    std::cout << "\n符号表:" << std::endl;
    for (const auto& entry : symbolTable) {
        std::cout << "[" << entry.first << "]" << std::endl;
        for (const auto& symbol : entry.second) {
            std::cout << "  type: " << symbol.type;
            if (!symbol.value.empty()) {
                std::cout << ", value: " << symbol.value;
            }
            std::cout << ", scope: " << symbol.scope;
            std::cout << ", level: " << symbol.level;
            if (symbol.isParameter) {
                std::cout << " (参数)";
            }
            std::cout << std::endl;
        }
    }
} 