#ifndef __AST_H__
#define __AST_H__

#include <fstream>
#include "Type.h"
#include <vector>

class SymbolEntry;

class Node
{
private:
    static int counter;
    int seq;
public:
    Node();
    int getSeq() const {return seq;};
    virtual void output(int level) = 0;
};

class ExprNode : public Node
{
protected:
    SymbolEntry *symbolEntry;
public:
    ExprNode(SymbolEntry *symbolEntry) : symbolEntry(symbolEntry){};
    ExprNode(){};
    SymbolEntry* getSymbolEntry(){return symbolEntry;}
};

class UnaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr;
public:
    enum {ADD, SUB, NOT};
    UnaryExpr(SymbolEntry *se, int op, ExprNode*expr) : ExprNode(se), op(op), expr(expr){};
    void output(int level);
};

class BinaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1, *expr2;
public:
    enum {ADD, SUB, MUL, DIV, MOD, AND, OR, NOT, LT, GT, LE, GE, EQ, NE};
    BinaryExpr(SymbolEntry *se, int op, ExprNode*expr1, ExprNode*expr2) : ExprNode(se), op(op), expr1(expr1), expr2(expr2){};
    void output(int level);
};

class AddExpr : public ExprNode
{
private:
    ExprNode *expr1, *expr2;
public:
    AddExpr(ExprNode*expr1, ExprNode*expr2) : expr1(expr1), expr2(expr2){};
    void output(int level);
};

class Constant : public ExprNode
{
public:
    Constant(SymbolEntry *se) : ExprNode(se){};
    void output(int level);
};

class Id : public ExprNode
{
private:
    ExprNode* expr;
public:
    Id(SymbolEntry *se) : ExprNode(se){};
    Id(SymbolEntry *se,ExprNode* expr) : ExprNode(se),expr(expr){};
    void output(int level);
};

class StmtNode : public Node
{};

class ExpStmt : public StmtNode
{
private:
    ExprNode *expr;
public:
    ExpStmt(ExprNode *expr):expr(expr){};
    void output(int level);
};

class EmptyStmt : public StmtNode
{
public:
    EmptyStmt(){};
    void output(int level);
};

class CompoundStmt : public StmtNode
{
private:
    StmtNode *stmt;
public:
    CompoundStmt(StmtNode *stmt) : stmt(stmt) {};
    CompoundStmt(){};
    void output(int level);
};

class SeqNode : public StmtNode
{
private:
    StmtNode *stmt1, *stmt2;
public:
    SeqNode(StmtNode *stmt1, StmtNode *stmt2) : stmt1(stmt1), stmt2(stmt2){};
    void output(int level);
};

class DeclStmt : public StmtNode
{
private:
StmtNode* stmt;
public:
    DeclStmt(StmtNode* stmt):stmt(stmt) {};
    DeclStmt(){};
    void output(int level);
};

class VarDeclStmt : public StmtNode
{
private:
StmtNode* stmt1,*stmt2;
public:
    VarDeclStmt(StmtNode* stmt1,StmtNode* stmt2):stmt1(stmt1),stmt2(stmt2){};
    void output(int level);
};

class VarDefStmt:public StmtNode
{
private:
    Id *id;
public:
    VarDefStmt(Id*id):id(id){};
    void output(int level);
};

class ConstVarDeclStmt : public StmtNode
{
private:
StmtNode* stmt1,*stmt2;
public:
    ConstVarDeclStmt(StmtNode* stmt1,StmtNode* stmt2):stmt1(stmt1),stmt2(stmt2){};
    void output(int level);
};
class ConstVarDefStmt:public StmtNode
{
private:
    Id* id;
public:
    ConstVarDefStmt(Id* id):id(id){};
    void output(int level);
};


class IfStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
public:
    IfStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt){};
    void output(int level);
};
class WhileStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
public:
    WhileStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt){};
    void output(int level);
};

class IfElseStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
    StmtNode *elseStmt;
public:
    IfElseStmt(ExprNode *cond, StmtNode *thenStmt, StmtNode *elseStmt) : cond(cond), thenStmt(thenStmt), elseStmt(elseStmt) {};
    void output(int level);
};

class ReturnStmt : public StmtNode
{
private:
    ExprNode *retValue;
public:
    ReturnStmt(ExprNode*retValue) : retValue(retValue) {};
    void output(int level);
};

class AssignStmt : public StmtNode
{
private:
    ExprNode *lval;
    ExprNode *expr;
public:
    AssignStmt(ExprNode *lval, ExprNode *expr) : lval(lval), expr(expr) {};
    void output(int level);
};

class FuncCall : public StmtNode
{
private:
    SymbolEntry *se;
public:
    FuncCall(SymbolEntry*se): se(se){}
    void output(int level);
};

class FunctionDef : public StmtNode
{
private:
    SymbolEntry *se;
    StmtNode *stmt;
    std::vector<Id*> params;
public:
    FunctionDef(SymbolEntry *se, StmtNode *stmt) : se(se), stmt(stmt){};
    FunctionDef(SymbolEntry* se):se(se){};
    void setStmtNode(StmtNode* stmt){this->stmt=stmt;};
    void addParam(Id* param)
    {
        params.push_back(param);
    }
    void output(int level);
};

class Ast
{
private:
    Node* root;
public:
    Ast() {root = nullptr;}
    void setRoot(Node*n) {root = n;}
    void output();
};

#endif
