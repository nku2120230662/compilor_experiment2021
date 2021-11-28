%code top{
    #include <iostream>
    #include <assert.h>
    #include "parser.h"
    extern Ast ast;
    int yylex();
    int yyerror( char const * );
}

%code requires {
    #include "Ast.h"
    #include "SymbolTable.h"
    #include "Type.h"
}

%{
    FunctionDef *func;
%}

%union {
    int itype;
    char* strtype;
    StmtNode* stmttype;
    ExprNode* exprtype;
    Type* type;
}

%start Program


%token RETURN
%token IF ELSE WHILE BREAK CONTINUE
%token INT VOID
%token CONST

%token COMMA
%token ASSIGN
%token OR
%token AND
%token EQ NE
%token LT GT LE GE
%token ADD SUB 
%token MUL DIV MOD
%token NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK SEMICOLON 
%token <strtype> ID 
%token <itype> INTEGER



%nterm <stmttype> Stmts Stmt AssignStmt BlockStmt  WhileStmt ReturnStmt DeclStmt VarDeclStmt VarDefStmt ConstVarDeclStmt ConstVarDefStmt FuncDef  IfStmt /*FuncCall*/ EmptyStmt ExpStmt
%nterm <exprtype> Exp Cond LVal PrimaryExp AddExp AddExps UnaryExp MulExp RelExp LAndExp LOrExp EqExp FuncFParams FuncFParam
%nterm <type> Type

%precedence THEN
%precedence ELSE
%%
//----------------------------编译单元---------------------------
Program
    : Stmts {
        ast.setRoot($1);
    }
    ;
//----------------------------类型---------------------------
Type
    : INT {
        $$ = TypeSystem::intType;
    }
    | VOID {
        $$ = TypeSystem::voidType;
    }
    ;
//----------------------------语句---------------------------
Stmts
    : Stmt {$$=$1;}
    | Stmts Stmt{
        $$ = new SeqNode($1, $2);
    }
    ;
Stmt
    : AssignStmt {$$=$1;}
    | BlockStmt {$$=$1;}
    | IfStmt {$$=$1;}
    | WhileStmt {$$=$1;}
    | ReturnStmt {$$=$1;}
    | DeclStmt {$$=$1;}
    | FuncDef {$$=$1;}
//    | FuncCall {$$=$1;}
    | EmptyStmt {$$=$1;}
    | ExpStmt {$$=$1;}
    ;

ExpStmt
    :
    AddExp SEMICOLON {
        $$=new ExpStmt($1);
    }

EmptyStmt
    : SEMICOLON {$$=new EmptyStmt();}

AssignStmt
    :
    LVal ASSIGN Exp SEMICOLON {
        $$ = new AssignStmt($1, $3);
    }
    ;
BlockStmt
    :   LBRACE 
        {identifiers = new SymbolTable(identifiers); /*go down,create symboltable*/} 
        Stmts RBRACE 
        {
            $$ = new CompoundStmt($3);
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev(); //last_level_st->current_level_st
            delete top;
        }
        |
        LBRACE RBRACE{$$=nullptr;}
    ;
IfStmt
    : IF LPAREN Cond RPAREN Stmt %prec THEN {
        $$ = new IfStmt($3, $5);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt {
        $$ = new IfElseStmt($3, $5, $7);
    }
    ;
WhileStmt
    : WHILE LPAREN Cond RPAREN Stmt {
        $$ = new WhileStmt($3, $5);
    }
    ;
ReturnStmt
    :
    RETURN Exp SEMICOLON{
        $$ = new ReturnStmt($2);
    }
    |
    RETURN SEMICOLON{
        $$=new ReturnStmt(nullptr);
    }
    ;
    
//----------------------------声明---------------------------
DeclStmt
    :
    Type VarDeclStmt SEMICOLON
    {
        $$=new DeclStmt($2);
    }
    |
    CONST Type ConstVarDeclStmt SEMICOLON
    {
        $$=new DeclStmt($3);
    }
    ;
VarDeclStmt
    :
    VarDeclStmt COMMA VarDefStmt 
    {
        $$=new VarDeclStmt($1,$3);
    }
    |
    VarDefStmt
    {
        $$=$1;
        
    }
    ;
VarDefStmt
    :
    ID{
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(TypeSystem::intType ,$1, identifiers->getLevel());
        identifiers->install($1, se);
        $$ = new VarDefStmt(new Id(se));
        delete []$1;
    }
    |
    ID ASSIGN Exp
    {
        SymbolEntry* se;
        se=new IdentifierSymbolEntry(TypeSystem::intType,$1,$3,identifiers->getLevel());
        identifiers->install($1,se);
        $$=new VarDefStmt(new Id(se,$3));
        delete[]$1;
    }
    |
    ID LBRACK RBRACK{ //array
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(TypeSystem::intType ,$1, identifiers->getLevel());
        identifiers->install($1, se);
        $$ = new VarDefStmt(new Id(se));
        delete []$1;
    }
    |
    ID LBRACK Exp RBRACK{ //array
        SymbolEntry* se;
        se=new IdentifierSymbolEntry(TypeSystem::intType,$1,$3,identifiers->getLevel());
        identifiers->install($1,se);
        $$=new VarDefStmt(new Id(se,$3));
        delete[]$1;
    }
    |
    ID LBRACK Exp RBRACK ASSIGN LBRACE AddExps RBRACE{ //array
        SymbolEntry* se;
        se=new IdentifierSymbolEntry(TypeSystem::intType,$1,$3,identifiers->getLevel());
        identifiers->install($1,se);
        $$=new VarDefStmt(new Id(se,$3));
        delete[]$1;
    }
    ;
ConstVarDeclStmt
    :
   ConstVarDefStmt COMMA ConstVarDeclStmt 
    {
        $$=new ConstVarDeclStmt($1,$3);
    }
    |
    ConstVarDefStmt
    {
        $$=$1;
        
    }
    ;
ConstVarDefStmt
    :
    ID ASSIGN Exp
    {
        SymbolEntry* se;
        se=new IdentifierSymbolEntry(TypeSystem::intType,$1,$3,identifiers->getLevel());
        identifiers->install($1,se);
        $$=new ConstVarDefStmt(new Id(se,$3));
        delete[]$1;
    }
    ;

/*FuncCall  //deleted
    :
    ID LPAREN RPAREN SEMICOLON {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new FuncCall(se);
        delete []$1;
    }
    |
    ID LPAREN AddExps RPAREN SEMICOLON {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new FuncCall(se);
        delete []$1;
    }
    ;
*/

FuncDef
    :
    Type ID 
    {
        Type *funcType;
        funcType = new FunctionType($1,{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, $2, identifiers->getLevel());
        identifiers->install($2, se);
        identifiers = new SymbolTable(identifiers);
        func = new FunctionDef(se);
    }
    LPAREN FuncFParams {} RPAREN
    BlockStmt
    {
        func->setStmtNode($8);
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        $$=func;
        delete top;
        delete []$2;
    }
    ;
FuncFParams
    :
    %empty{$$=nullptr;}  
    |
    FuncFParam
    |
    FuncFParams COMMA FuncFParam
    ;
FuncFParam
    :
    Type ID
    {
        SymbolEntry *se;
        se = new IdentifierSymbolEntry($1 ,$2, identifiers->getLevel());
        identifiers->install($2, se);
        func->addParam(new Id(se));
        delete []$2;

    }
    |
    Type ID LBRACK RBRACK{ //array
        SymbolEntry *se;
        se = new IdentifierSymbolEntry($1 ,$2, identifiers->getLevel());
        identifiers->install($2, se);
        func->addParam(new Id(se));
        delete []$2;
    }
    |
    Type ID LBRACK Exp RBRACK{ //array
        SymbolEntry *se;
        se = new IdentifierSymbolEntry($1 ,$2, identifiers->getLevel());
        identifiers->install($2, se);
        func->addParam(new Id(se));
        delete []$2;
    }
    ;
//----------------------------表达式---------------------------
Exp
    :
    AddExp {$$ = $1;}
    ;
AddExps:
    AddExp
    |
    AddExps COMMA AddExp
    ;
Cond
//条件表达式 delete EqExp
    :
    LOrExp {$$ = $1;}
    ;
AddExp
//加减表达式
    :
    MulExp {$$ = $1;}
    |
    AddExp ADD MulExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::ADD, $1, $3);
    }
    |
    AddExp SUB MulExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::SUB, $1, $3);
    }
    ;
MulExp
//乘除模表达式
    :
    UnaryExp {$$ = $1;}
    |
    MulExp MUL UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MUL, $1, $3);
    }
    |
    MulExp DIV UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::DIV, $1, $3);
    }
    |
    MulExp MOD UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MOD, $1, $3);
    }
    ;
UnaryExp
//一元表达式
    :
    PrimaryExp {$$ = $1;}
    |
    ADD UnaryExp {
        SymbolEntry *se=$2->getSymbolEntry();
        $$=new UnaryExpr(se,UnaryExpr::ADD,$2);
    }
    |
    SUB UnaryExp {
        SymbolEntry *se=$2->getSymbolEntry();
        $$=new UnaryExpr(se,UnaryExpr::SUB,$2);
    }
    |
    NOT UnaryExp {
        SymbolEntry *se=$2->getSymbolEntry();
        $$=new UnaryExpr(se,UnaryExpr::NOT,$2);
    }
//    |
//    ID LBRACK INTEGER RBRACK { //array
//
//    }
    ; 
PrimaryExp
//基本表达式
    :
    LVal {
        $$ = $1;
    }
    | 
    INTEGER {
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::intType, $1);
        $$ = new Constant(se);
    }
    |
    LPAREN Exp RPAREN{
        $$ = $2;
    }
    |
    ID LPAREN AddExps RPAREN {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se);
        delete []$1;
    }
    |
    ID LPAREN RPAREN { //funccall
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se);
        delete []$1;
    }
    ;
LVal
//左值表达式
    : ID {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se);
        delete []$1;
    }
    |
    ID LBRACK Exp RBRACK {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se);
        delete []$1;
    }
    ;
LOrExp
    :
    LAndExp {$$ = $1;}
    |
    LOrExp OR LAndExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::OR, $1, $3);
    }
    ;
LAndExp
    :
    EqExp {$$ = $1;}
    |
    LAndExp AND EqExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::AND, $1, $3);
    }
    ;
EqExp
    :
    RelExp{$$=$1;}
    |
    EqExp EQ RelExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::EQ, $1, $3);
    }
    |
    EqExp NE RelExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::NE, $1, $3);
    }
    ;
RelExp
//关系表达式
    :
    AddExp {$$ = $1;}
    |
    RelExp LT AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LT, $1, $3);
    }
    |
    RelExp GT AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GT, $1, $3);
    }
    |
    RelExp LE AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LE, $1, $3);
    }
    |
    RelExp GE AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GE, $1, $3);
    }
    ;


%%

int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
