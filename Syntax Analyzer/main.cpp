#include "recursive_parser.h"
#include "predictive_parser.h"
#include <iostream>
#include <fstream>
#include <string>

// 基本测试用例
const std::string testProgram1 = R"(
PROGRAM test1;
CONST max = 100;
VAR x, y: INTEGER;
BEGIN
    x := 1;
    y := 2;
    IF x < y THEN
        x := x + 1;
    WHILE x < max DO
        x := x + 1;
END.
)";

// 测试过程定义和作用域
const std::string testProgram2 = R"(
PROGRAM test2;
CONST max = 100;
VAR x, y: INTEGER;
PROCEDURE swap(a, b: INTEGER);
VAR temp: INTEGER;
BEGIN
    temp := a;
    a := b;
    b := temp;
END;
BEGIN
    x := 1;
    y := 2;
    swap(x, y);
END.
)";

// 测试嵌套过程和变量重名
const std::string testProgram3 = R"(
PROGRAM test3;
CONST max = 100;
VAR x: INTEGER;
PROCEDURE outer(x: INTEGER);
VAR y: INTEGER;
    PROCEDURE inner(y: INTEGER);
    VAR x: INTEGER;
    BEGIN
        x := y + 1;
    END;
BEGIN
    y := x + 1;
    inner(y);
END;
BEGIN
    x := 1;
    outer(x);
END.
)";

// 测试类型检查和常量定义
const std::string testProgram4 = R"(
PROGRAM test4;
CONST max = 100;
    min = 0;
VAR x: INTEGER;
    y: LONG;
PROCEDURE check(a: INTEGER; b: LONG);
BEGIN
    IF a < max THEN
        x := a;
    IF b > min THEN
        y := b;
END;
BEGIN
    x := max;
    y := min;
    check(x, y);
END.
)";

// 测试词法分析器的程序
const std::string lexerTestCode = 
"PROGRAM MyTest;\n"
"CONST a = 10;\n"
"VAR b: INTEGER;\n"
"BEGIN\n"
"  b := a + 5;\n"
"  IF b > 10 THEN\n"
"    b := b - 1;\n"
"END.\n";

void testRecursiveParser(const std::string& program, const std::string& testName) {
    std::cout << "\n测试递归子程序法语法分析器 - " << testName << "：" << std::endl;
    RecursiveParser parser;
    bool result = parser.parse(program);
    if (result) {
        std::cout << "语法分析结果：是" << std::endl;
        parser.printSymbolTable();
    } else {
        std::cout << "语法分析结果：否" << std::endl;
    }
}

void testPredictiveParser(const std::string& program, const std::string& testName) {
    std::cout << "\n测试预测分析法语法分析器 - " << testName << "：" << std::endl;
    PredictiveParser parser;
    bool result = parser.parse(program);
    if (result) {
        std::cout << "语法分析结果：是" << std::endl;
        parser.printSymbolTable();
    } else {
        std::cout << "语法分析结果：否" << std::endl;
    }
}

void testLexer(const std::string& sourceCode, const std::string& testName) {
    std::cout << "\n测试词法分析器 - " << testName << "：" << std::endl;
    
    // 将源代码写入临时文件
    std::ofstream tempFile("temp_lexer_test.txt");
    tempFile << sourceCode;
    tempFile.close();
    
    // 创建词法分析器实例
    Lexer lexer("temp_lexer_test.txt");
    
    // 获取并打印所有Token
    std::cout << "行号\t列号\t内码\t词素" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    Token token;
    do {
        token = lexer.nextToken();
        std::cout << token.lineNo << "\t" << token.columnNo << "\t" 
                 << token.code << "\t" << token.value << std::endl;
    } while (token.code != 0); // 直到文件结束
}

void testLexerOnly() {
    std::cout << "\n词法分析器专项测试：" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    
    // 将测试代码写入文件
    std::ofstream codeFile("lexer_test_code.txt");
    codeFile << lexerTestCode;
    codeFile.close();
    
    std::cout << "测试代码：\n" << lexerTestCode << std::endl;
    
    try {
        // 创建词法分析器并分析代码
        Lexer lexer("lexer_test_code.txt");
        
        std::cout << "\n词法分析结果：" << std::endl;
        std::cout << "行号\t列号\t内码\t词素" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        
        Token token;
        do {
            token = lexer.nextToken();
            std::cout << token.lineNo << "\t" << token.columnNo << "\t" 
                     << token.code << "\t" << token.value << std::endl;
        } while (token.code != 0); // 直到文件结束
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "Pascal子集语言语法分析器测试程序" << std::endl;
    std::cout << "=================================" << std::endl;
    
    // 专项测试词法分析器
    testLexerOnly();
    
    // 测试词法分析器
    testLexer(testProgram1, "基本词法测试");
    
    // 测试基本语法
    testRecursiveParser(testProgram1, "基本语法测试");
    testPredictiveParser(testProgram1, "基本语法测试");
    
    // 测试过程定义和作用域
    testRecursiveParser(testProgram2, "过程定义和作用域测试");
    testPredictiveParser(testProgram2, "过程定义和作用域测试");
    
    // 测试嵌套过程和变量重名
    testRecursiveParser(testProgram3, "嵌套过程和变量重名测试");
    testPredictiveParser(testProgram3, "嵌套过程和变量重名测试");
    
    // 测试类型检查和常量定义
    testRecursiveParser(testProgram4, "类型检查和常量定义测试");
    testPredictiveParser(testProgram4, "类型检查和常量定义测试");
    
    return 0;
} 