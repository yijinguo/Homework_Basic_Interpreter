/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);
int StringToInt(string s);
bool ifInteger(string s);
void execute(string input,string line);

/* Main program */
bool quit = false;
EvalState state;
Program program;
int main() {
    while (!quit) {
        try {
            string input;
            cin >> input;
            string line = getLine();
            if (input.empty()) {
                return 0;
            } else if (input[0] >= '0' && input[0] <= '9') { //首为数字
                int lineNumber;
                lineNumber = StringToInt(input);
                if (line.empty()) {
                    if (program.count(lineNumber) != 0) {
                        program.removeSourceLine(lineNumber);
                    }
                } else {
                    program.addSourceLine(lineNumber, line);
                }
            } else { //首为指令:input
                execute(input, line);
            }
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

bool ifEnd = false;
bool OperatorNumChange = false;
bool ifCout = true;
int operator_num;
int outValue;
TokenScanner scannerP;
void execute(string input,string line) { //input指令，line不含指令
    if (input == "QUIT") {
        quit = true;
    } else if (input == "LIST") {
        if (!program.empty()) {
            int lineNumber = program.getFirstLineNumber();
            while (program.getNextLineNumber(lineNumber) != -1) {
                cout << lineNumber << program.getSourceLine(lineNumber) << endl;
                lineNumber = program.getNextLineNumber(lineNumber);
            }
            cout << lineNumber << program.getSourceLine(lineNumber) << endl;
        }
    } else if (input == "LET") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        string var, op;
        var = scanner.nextToken();
        op = scanner.nextToken();
        Expression *exp = parseExp(scanner);
        if (var == "LET") {
            cout << "SYNTAX ERROR" << endl;
        } else {
            if (exp->DivideZero(state)) {
                cout << "DIVIDE BY ZERO" << endl;
            } else if (!exp->isDefined(state)) {
                cout << "VARIABLE NOT DEFINED" << endl;
            } else {
                int value = exp->eval(state);
                delete exp;
                state.setValue(var, value);
            }
        }
    } else if (input == "CLEAR") {
        state.clear();
        program.clear();
    } else if (input == "PRINT") {
        processLine(line, program, state);
        if (ifCout) cout << outValue <<endl;
        ifCout = true;
    } else if (input == "INPUT") {
        cout << " ? ";
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        string var = scanner.nextToken();
        string word = getLine();
        while (!ifInteger(word)) {
            cout << "INVALID NUMBER" << endl << " ? ";
            word = getLine();
        }
        int value = StringToInt(word);
        state.setValue(var, value);
    } else if (input == "RUN") {
        ifEnd = false;
        operator_num = program.getFirstLineNumber();
        while (!ifEnd && program.getNextLineNumber(operator_num) != -1) {
            string operate = program.getSourceLine(operator_num);
            TokenScanner scanner;
            scanner.ignoreWhitespace();
            scanner.scanNumbers();
            scanner.setInput(operate);
            string input = scanner.nextToken();
            operate.erase(0,input.length() + 1);
            execute(input,operate);
            if (!OperatorNumChange) {
                operator_num = program.getNextLineNumber(operator_num);
            } else {
                OperatorNumChange = false;
            }
        }
        if (!ifEnd) {
            string operate = program.getSourceLine(operator_num);
            TokenScanner scanner;
            scanner.ignoreWhitespace();
            scanner.scanNumbers();
            scanner.setInput(operate);
            string input = scanner.nextToken();
            operate.erase(0, input.length() + 1);
            execute(input, operate);
        }
    } else if (input == "END") {
        ifEnd = true;
    } else if (input == "GOTO") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        string l = scanner.nextToken();
        int lineNumber = StringToInt(l);
        if (program.getSourceLine(lineNumber).empty()){
            cout << "LINE NUMBER ERROR" << endl;
        } else {
            OperatorNumChange = true;
            operator_num = lineNumber;
        }
    } else if (input == "IF") {
        processLine(line,program,state);
        int left = outValue;
        string op = scannerP.nextToken();
        scannerP.ignoreWhitespace();
        scannerP.scanNumbers();
        Expression *exp = parseExp(scannerP);
        outValue = exp->eval(state);
        int right = outValue;
        bool executeThen = false;
        if (op == "=" && left == right) {
            executeThen = true;
        } else if (op == "<" && left < right) {
            executeThen = true;
        } else if (op == ">" && left > right) {
            executeThen = true;
        } else {
            executeThen = false;
        }
        if (executeThen) {
            string THEN = scannerP.nextToken();
            string nextLineName = scannerP.nextToken();
            int nextLineNumber = StringToInt(nextLineName);
            OperatorNumChange = true;
            operator_num = nextLineNumber;
        }
    }
}

void processLine(string line, Program &program, EvalState &state) { // line中不含指令
    scannerP.ignoreWhitespace();
    scannerP.scanNumbers();
    scannerP.setInput(line);
    Expression *exp = parseExp(scannerP);
    ExpressionType type = exp->getType();
    if (type == COMPOUND && exp->DivideZero(state)){
        cout << "DIVIDE BY ZERO" << endl;
        ifCout = false;
    } else if (type == IDENTIFIER && !exp->isDefined(state)){
        cout << "VARIABLE NOT DEFINED" << endl;
        ifCout = false;
    } else {
        outValue = exp->eval(state);
        delete exp;
    }
}

int StringToInt(string s){
    int value = 0;
    int time = 1;
    if (s[0] != '-') {
        for (int i = s.length() - 1; i >= 0; --i) {
            value += (s[i] - 48) * time;
            time *= 10;
        }
    } else {
        for (int i = s.length() - 1; i >= 1; --i) {
            value += (s[i] - 48) * time;
            time *= 10;
        }
        value = (-1) * value;
    }
    return value;
}

bool ifInteger(string s){
    if (s[0] != '-') {
        for (int i = 0; i < s.length(); ++i) {
            if (!(s[i] >= '0' && s[i] <= '9')) return false;
        }
    } else {
        for (int i = 1; i < s.length(); ++i) {
            if (!(s[i] >= '0' && s[i] <= '9')) return false;
        }
    }
    return true;
}