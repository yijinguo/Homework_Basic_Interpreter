/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"

using namespace std;

Program::Program() {
    source_line.clear();
}

Program::~Program() {
    // Replace this stub with your own code
}

void Program::clear() {
    source_line.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    source_line[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    source_line.erase(lineNumber);
    parsed_representation.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    if (source_line.count(lineNumber) == 1) {
        return source_line[lineNumber];
    } else {
        return "";
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    parsed_representation[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    return parsed_representation[lineNumber];
}

int Program::getFirstLineNumber() {
    if (source_line.empty()) {
        return -1;
    } else {
        auto iter = source_line.begin();
        return iter->first;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    auto iter = source_line.find(lineNumber);
    iter++;
    if (iter != source_line.end()) {
        return iter->first;
    } else {
        return -1;
    }
}

bool Program::empty(){
    if (source_line.empty())
        return true;
    else
        return false;
}

int Program::count(int lineNumber) {
    return source_line.count(lineNumber);
}