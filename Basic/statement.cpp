/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
#include "parser.h"

using namespace std;

/* Implementation of the Statement class */

bool ifEnd = false;
bool OperatorNumChange = false;
bool ifCout = true;
int operator_num;
int outValue;
TokenScanner scannerP;

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

namespace {
    int StringToInt(string s) {
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

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

RemExecute::RemExecute(string order) {
    this->order = order;
}
void RemExecute::execute(string line, Program &program, EvalState &state) {
    /*Empty*/
}

LetExecute::LetExecute(string order) {
    this->order = order;
}
void LetExecute::execute(string line, Program &program, EvalState &state) {
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
}

PrintExecute::PrintExecute(string order) {
    this->order = order;
}
void PrintExecute::execute(string line, Program &program, EvalState &state) {
    processLine(line, program, state);
    if (ifCout) cout << outValue <<endl;
    ifCout = true;
}

InputExecute::InputExecute(string order) {
    this->order = order;
}
void InputExecute::execute(string line, Program &program, EvalState &state) {
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
}

EndExecute::EndExecute(string order) {
    this->order = order;
}
void EndExecute::execute(string line, Program &program, EvalState &state) {
    ifEnd = true;
}

GotoExecute::GotoExecute(string order) {
    this->order = order;
}
void GotoExecute::execute(string line, Program &program, EvalState &state) {
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
}

IfExecute::IfExecute(string order) {
    this->order = order;
}
void IfExecute::execute(string line, Program &program, EvalState &state) {
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

RunExecute::RunExecute(string order) {
    this->order = order;
}
void RunExecute::execute(string line, Program &program, EvalState &state) {
    ifEnd = false;
    operator_num = program.getFirstLineNumber();
    while (!ifEnd && program.getNextLineNumber(operator_num) != -1) {
        string operateLine = program.getSourceLine(operator_num);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(operateLine);
        string input = scanner.nextToken();
        operateLine.erase(0,input.length() + 1);
        Statement *stmt = getStatement(input);
        stmt->execute(operateLine,program,state);
        if (!OperatorNumChange) {
            operator_num = program.getNextLineNumber(operator_num);
        } else {
            OperatorNumChange = false;
        }
    }
    if (!ifEnd) {
        string operateLine = program.getSourceLine(operator_num);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(operateLine);
        string input = scanner.nextToken();
        operateLine.erase(0, input.length() + 1);
        Statement *stmt = getStatement(input);
        stmt->execute(operateLine,program,state);
    }
}

ListExecute::ListExecute(string order) {
    this->order = order;
}
void ListExecute::execute(string line, Program &program, EvalState &state) {
    if (!program.empty()) {
        int lineNumber = program.getFirstLineNumber();
        while (program.getNextLineNumber(lineNumber) != -1) {
            cout << lineNumber << program.getSourceLine(lineNumber) << endl;
            lineNumber = program.getNextLineNumber(lineNumber);
        }
        cout << lineNumber << program.getSourceLine(lineNumber) << endl;
    }
}

ClearExecute::ClearExecute(string order) {
    this->order = order;
}
void ClearExecute::execute(string line, Program &program, EvalState &state) {
    state.clear();
    program.clear();
}



