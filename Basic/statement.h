/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include "program.h"
#include "evalstate.h"
#include "exp.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

enum StatementType {REM, LET, PRINT, INPUT, END, GOTO, IF, RUN, LIST, CLEAR, HELP};

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(std::string line, Program &program, EvalState &state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

//rem

class RemExecute : public Statement {

private:

    std::string order;

public:

    RemExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//let

class LetExecute : public Statement {

private:

    std::string order;

public:

    LetExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//print

class PrintExecute : public Statement {

private:

    std::string order;

public:

    PrintExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//input

class InputExecute : public Statement {

private:

    std::string order;

public:

    InputExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//end

class EndExecute : public Statement {

private:

    std::string order;

public:

    EndExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//goto

class GotoExecute : public Statement {

private:

    std::string order;

public:

    GotoExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//if

class IfExecute : public Statement {

private:

    std::string order;

public:

    IfExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//run

class RunExecute : public Statement {

private:

    std::string order;

public:

    RunExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//list

class ListExecute : public Statement {

private:

    std::string order;

public:

    ListExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

//clear

class ClearExecute : public Statement {

private:

    std::string order;

public:

    ClearExecute(std::string order);

    virtual void execute(std::string line, Program &program, EvalState &state);

};

#endif


