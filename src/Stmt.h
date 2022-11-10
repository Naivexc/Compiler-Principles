#ifndef STMT_H
#define STMT_H

#include "ast.h"

class StmtCase0 // lval=exp;
{
public:
    std::unique_ptr<BaseAST> l_val;
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase1 //;
{
public:
};

class StmtCase2 // exp;
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase3 // block
{
public:
    std::unique_ptr<BaseAST> block;
};

class StmtCase4 // if
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt;
    std::string label_then;
    std::string label_end;
};

class StmtCase5 // if else
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt_if;
    std::unique_ptr<BaseAST> stmt_else;
    std::string label_then;
    std::string label_else;
    std::string label_end;
};

class StmtCase6 // while
{
public:
    std::unique_ptr<BaseExpAST> exp;
    std::unique_ptr<BaseAST> stmt;
    std::string label_while;
    std::string label_while_true;
    std::string label_while_false;
};

class StmtCase7 // break
{
public:
    BaseAST *last_while_ins;
};

class StmtCase8 // continue
{
public:
    BaseAST *last_while_ins;
};

class StmtCase9
{
public:
    std::unique_ptr<BaseExpAST> exp;
};

class StmtCase10
{
public:
    // empty
};

#endif