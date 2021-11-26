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
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"


using namespace std;

/* Function prototypes */

namespace {
    int StringToInt(string s);
}

/* Main program */
int main() {
    EvalState state;
    Program program;
    while (true) {
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
                    TokenScanner p;
                    p.ignoreWhitespace();
                    p.scanNumbers();
                    p.setInput(line);
                    string first = p.nextToken();
                    if (first == "QUIT") {
                        break;
                    } else {
                        program.addSourceLine(lineNumber, line);
                    }
                }
            } else { //首为指令:input
                if (input == "QUIT") {
                    break;
                } else {
                    Statement *stmt = getStatement(input);
                    stmt->execute(line, program, state);
                }
            }
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
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



