/**
 * @file main.cpp
 * @brief Pascal子集词法分析器的主程序
 * @author 宁波大学 PTB
 * @github https://github.com/CraneBW/-Compiler-Principles
 */

/**
 * @file main.cpp
 * @brief Pascal子集词法分析器的主程序
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <string.h>
#include "lexer.h"

void printUsage() {
    std::cout << "用法：lexer <输入文件> [输出文件]" << std::endl;
    std::cout << "  <输入文件>: Pascal子集源代码文件" << std::endl;
    std::cout << "  [输出文件]: 可选的输出文件，默认为控制台输出" << std::endl;
}

void processFile(const std::string& inputFile, const std::string& outputFile = "") {
    // 打开输入文件
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "错误：无法打开输入文件 " << inputFile << std::endl;
        return;
    }
    
    // 创建词法分析器
    Lexer lexer(inFile);
    
    // 获取所有Token
    std::vector<Token> tokens = lexer.getAllTokens();
    
    // 准备输出
    std::ostream* out = &std::cout;  // 默认输出到控制台
    std::ofstream outFile;
    
    if (!outputFile.empty()) {
        outFile.open(outputFile);
        if (!outFile) {
            std::cerr << "错误：无法打开输出文件 " << outputFile << std::endl;
            return;
        }
        out = &outFile;
    }
    
    // 输出Token序列
    *out << "===== Token序列 =====" << std::endl;
    *out << std::right
         << std::setw(10) << "行号" << std::setw(10) << "列号" << std::setw(15) << "类型"
         << std::setw(15) << "内码" << std::setw(20) << "值" << std::endl;

    for (const Token& token : tokens) {
        *out << std::right
             << std::setw(10) << token.getLine() 
             << std::setw(10) << token.getColumn() 
             << std::setw(15) << token.getTypeName() 
             << std::setw(15) << token.getType() 
             << std::setw(20) << token.getValue() << std::endl;
    }
    
    // 输出符号表
    SymbolTable symbolTable = lexer.getSymbolTable();
    
    *out << std::endl;
    *out << "========================= 符号表 ======================================" << std::endl;
    *out << std::right
         << std::setw(20) << "名称" << std::setw(15) << "类型"
         << std::setw(15) << "地址" << std::setw(15) << "是否常量" << std::endl;

    for (const Symbol& symbol : symbolTable.getAllSymbols()) {
        std::string typeStr;
        
        switch (symbol.getType()) {
            case TYPE_INTEGER:
                typeStr = "INTEGER";
                break;
            case TYPE_LONG:
                typeStr = "LONG";
                break;
            case TYPE_PROCEDURE:
                typeStr = "PROCEDURE";
                break;
            default:
                typeStr = "UNKNOWN";
        }

        *out<< std::right
            << std::setw(20) << symbol.getName()
            << std::setw(15) << typeStr
            << std::setw(15) << symbol.getAddress()
            << std::setw(15) << (symbol.isConst() ? "是" : "否") << std::endl;
    }
    
    // 关闭文件
    if (outFile.is_open()) {
        outFile.close();
    }
    
    std::cout << "词法分析完成！" << std::endl;
}

int main(int argc, char* argv[]) {

    const std::string COLOR_HEADER = "\033[1;32m"; // 亮绿色
    const std::string COLOR_AUTHOR = "\033[1;36m"; // 青色
    const std::string COLOR_LINK = "\033[4;34m";   // 蓝色带下划线
    const std::string COLOR_RESET = "\033[0m";     // 重置样式

    // 检查命令行参数


    if (argc == 1 && strcmp(argv[0], "exit")) {
        std::cout << "Bye!" << std::endl;
        return 1;
    }

    if (argc < 2 || argc > 3) {
        printUsage();
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = (argc == 3) ? argv[2] : "";


    std::cout << "\n"
              << COLOR_HEADER
              << "==========================================" << COLOR_RESET << "\n"
              << COLOR_AUTHOR << "代码作者: " << COLOR_RESET
              << "宁波大学 PTB\n"
              << COLOR_AUTHOR << "开源地址: " << COLOR_RESET
              << COLOR_LINK << "https://github.com/CraneBW/-Compiler-Principles" << COLOR_RESET << "\n"
              << COLOR_HEADER << "==========================================" << COLOR_RESET
              << std::endl;

    processFile(inputFile, outputFile);
    
    return 0;
} 