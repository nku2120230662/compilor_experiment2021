#include "Ast.h"
#include "SymbolTable.h"
#include <string>
#include "Type.h"

extern FILE *yyout;
int Node::counter = 0;

Node::Node()
{
    seq = counter++;
}

void Ast::output()
{
    fprintf(yyout, "program\n");
    if(root != nullptr)
        root->output(4);
}

void UnaryExpr::output(int level)
{
    std::string op_str;
    switch(op){
        case ADD:
            op_str = "uadd";
            break;
        case SUB:
            op_str = "usub";
            break;
        case NOT:
            op_str = "not";
            break;
    }
    fprintf(yyout, "%*cUnaryExpr\top: %s\n", level, ' ', op_str.c_str());
    expr->output(level + 4); 
}

void BinaryExpr::output(int level)
{
    std::string op_str;
    switch(op)
    {
        case ADD:
            op_str = "add";
            break;
        case SUB:
            op_str = "sub";
            break;
        case MUL:
            op_str = "mul";
            break;
        case DIV:
            op_str = "div";
            break; 
        case MOD:
            op_str = "mod";
            break; 
        case AND:
            op_str = "and";
            break;
        case OR:
            op_str = "or";
            break;
        case NOT:
            op_str = "not";
            break;
        case LT:
            op_str = "lt";
            break;
        case GT:
            op_str = "gt";
            break;
        case LE:
            op_str = "le";
            break;
        case GE:
            op_str = "ge";
            break;
        case EQ:
            op_str = "eq";
            break;
        case NE:
            op_str = "ne";
            break;
    }
    fprintf(yyout, "%*cBinaryExpr\top: %s\n", level, ' ', op_str.c_str());
    expr1->output(level + 4);  //level means the count of " " in the beginning of current line
    expr2->output(level + 4);
}

void AddExpr::output(int level)
{
    fprintf(yyout, "%*cAddExpr\n", level, ' ');
    expr1->output(level + 4);  
    expr2->output(level + 4);
}

void ExpStmt::output(int level){
    fprintf(yyout, "%*cExpStmt\n", level, ' ');
    expr->output(level + 4);  
}

void EmptyStmt::output(int level){
    fprintf(yyout, "%*cEmptyStmt\n", level, ' ');
}

void Constant::output(int level)
{
    std::string type, value;
    type = symbolEntry->getType()->toStr();
    value = symbolEntry->toStr();
    fprintf(yyout, "%*cIntegerLiteral\tvalue: %s\ttype: %s\n", level, ' ',
            value.c_str(), type.c_str());
}

void Id::output(int level)
{
    std::string name, type;
    int scope;
    name = symbolEntry->toStr();
    type = symbolEntry->getType()->toStr();
    scope = dynamic_cast<IdentifierSymbolEntry*>(symbolEntry)->getScope();
    fprintf(yyout, "%*cId\tname: %s\tscope: %d\ttype: %s\n", level, ' ',
            name.c_str(), scope, type.c_str());
    if(expr){
        expr->output(level+4);
    }
}

void CompoundStmt::output(int level)
{
    fprintf(yyout, "%*cCompoundStmt\n", level, ' ');
    if(stmt!=nullptr) stmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNoneStmt\n", level+4, ' ');
    }
}

void SeqNode::output(int level)
{
    fprintf(yyout, "%*cSequence\n", level, ' ');
    stmt1->output(level + 4);
    stmt2->output(level + 4);
}

//声明、定义
void DeclStmt::output(int level)
{
    fprintf(yyout, "%*cDeclStmt\n", level, ' ');
    stmt->output(level + 4);
}
void VarDeclStmt::output(int level)
{
    fprintf(yyout, "%*cVarDeclStmt\n", level, ' ');
    stmt1->output(level + 4);
    stmt2->output(level+4);
}
void VarDefStmt::output(int level)
{
    fprintf(yyout, "%*cVarDefStmt\n", level, ' ');
    id->output(level + 4);
}
void ConstVarDeclStmt::output(int level)
{
    fprintf(yyout, "%*cConstarDeclStmt\n", level, ' ');
    stmt1->output(level + 4);
    stmt2->output(level+4);
}
void ConstVarDefStmt::output(int level)
{
    fprintf(yyout, "%*cConstVarDefStmt\n", level, ' ');
    id->output(level + 4);
}
void FuncCall::output(int level)
{
    std::string name,type;
    name=se->toStr();
    type=se->getType()->toStr();
    fprintf(yyout, "%*cFunctionCall function name: %s, type: %s\n", level , ' ',
            name.c_str(), type.c_str());
}
void FunctionDef::output(int level)
{
    std::string name, type;
    name = se->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "%*cFunctionDefine function name: %s, type: %s\n", level , ' ', 
            name.c_str(), type.c_str());
    // output params
    std::string name0,type0;
    for(long unsigned int i=0;i<params.size();i++){
        IdentifierSymbolEntry*se =dynamic_cast<IdentifierSymbolEntry*>(params[i]->getSymbolEntry());
        name0=se->toStr();
        type0=se->getType()->toStr();
        fprintf(yyout, "%*cParam %ld : name: %s, type: %s\n", level+4 , ' ', i+1 ,
            name0.c_str(), type0.c_str());
    }
    if(stmt!=nullptr) stmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNo Stmts\n", level+4 , ' ');
    }
}
//逻辑语句
void IfStmt::output(int level)
{
    fprintf(yyout, "%*cIfStmt\n", level, ' ');
    cond->output(level + 4);
    if(thenStmt!=nullptr) thenStmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNo Stmts\n", level+4 , ' ');
    }
}

void IfElseStmt::output(int level)
{
    fprintf(yyout, "%*cIfElseStmt\n", level, ' ');
    cond->output(level + 4);
    if(thenStmt!=nullptr) thenStmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNo Stmts\n", level+4 , ' ');
    }
    if(elseStmt!=nullptr) elseStmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNo Stmts\n", level+4 , ' ');
    }
}

void WhileStmt::output(int level)
{
    fprintf(yyout, "%*cWhileStmt\n", level, ' ');
    cond->output(level + 4);
    if(thenStmt!=nullptr) thenStmt->output(level + 4);
    else{
        fprintf(yyout, "%*cNo Stmts\n", level+4, ' ');
    }
}

void ReturnStmt::output(int level)
{
    fprintf(yyout, "%*cReturnStmt\n", level, ' ');
    if(retValue!=nullptr)retValue->output(level + 4);
    else{
        fprintf(yyout, "%*cReturn None\n", level+4, ' ');
    }
}

void AssignStmt::output(int level)
{
    fprintf(yyout, "%*cAssignStmt\n", level, ' ');
    lval->output(level + 4);
    expr->output(level + 4);
}

