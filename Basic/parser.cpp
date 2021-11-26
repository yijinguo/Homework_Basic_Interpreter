/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "program.h"
#include "statement.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"

using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Statement *getStatement(std::string order){
    if (order == "REM") {
        return new RemExecute("REM");
    } else if (order == "LET") {
        return new LetExecute("LET");
    } else if (order == "PRINT") {
        return new PrintExecute("PRINT");
    } else if (order == "INPUT") {
        return new InputExecute("INPUT");
    } else if (order == "END") {
        return new EndExecute("END");
    } else if (order == "GOTO") {
        return new GotoExecute("GOTO");
    } else if (order == "IF") {
        return new IfExecute("IF");
    } else if (order == "RUN") {
        return new RunExecute("RUN");
    } else if (order == "LIST") {
        return new ListExecute("LIST");
    } else {
        return new ClearExecute("CLEAR");
    }
}

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    //if (scanner.hasMoreTokens()) {
    //    error("parseExp: Found extra token: " + scanner.nextToken());
    //}
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    string token;
    Expression *rhs;
    token = scanner.nextToken();
    while (token != "=" || token != ">" || token != "<") {
        if (token.empty()) break;
        int newPrec = precedence(token);//符号优先序
        if (newPrec <= prec) break;
        rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
        token = scanner.nextToken();
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized(括号内的) subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));
    if (token != "(") error("Illegal term in expression");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
    if (token == "=" || token == "<" || token == ">") return 0;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}
