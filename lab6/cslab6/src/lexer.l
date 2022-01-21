%option noyywrap
%option yylineno
%{
     
    #ifdef ONLY_FOR_LEX
    #else
    #define YYSTYPE types
    #endif

    #define YY_NO_UNPUT
    #define YY_NO_INPUT
    #include <string>
    #include <stdio.h>

    #include <iostream>
    #include <iomanip>
    #include <fstream>
    #include <math.h>
    using namespace std;

    extern FILE *yyin; 
    extern FILE *yyout;
    extern bool dump_tokens;

    void DEBUG_FOR_LAB4(std::string s){
        fprintf(yyout, "%-12d%-12s%-12s\n", (yylineno+1)/2 , s.c_str(), yytext);
    }
%}


VOID "void"
INT "int"
CONST "const"

SEMICOLON ";"
COMMA ","

LPAREN "("
RPAREN ")"
LBRACK "["
RBRACK "]"
LBRACE "{"
RBRACE "}"

IF "if"
ELSE "else"
WHILE "while"
RETURN "return"
BREAK "break"
CONTINUE "continue"

NE "!="
LE "<="
GE ">="
EQ "=="
GT ">"
LT "<"

AND "&&"
OR "||"
NOT "!"

ASSIGN "="

ADD "+"
SUB "-"
MUL "*"
DIV "/"
MOD "%"

commentbegin "/*"
commentelement .
commentend "*/"
%x COMMENT

commentlinebegin "//"
commentlineelement .
commentlineend \n
%x COMMENTLINE

DECIMIAL ([1-9][0-9]*|0)
HEXNUMBER 0[Xx][0-9a-fA-F]+
OCTNUMBER 0[0-7]+

ID [[:alpha:]_][[:alpha:][:digit:]_]*
EOL (\r\n|\n|\r)
WHITE [\t ]


%%
{COMMA} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("COMMA");
    #else
        return COMMA;
    #endif 
}
{VOID} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("VOID");
    #else
        return VOID;
    #endif 
}
{INT} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("INT");
    #else
        return INT;
    #endif
}
{CONST} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("CONST");
    #else
        return CONST;
    #endif
}

{EQ} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("EQ");
    #else
        return EQ;
    #endif
}
{NE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NE");
    #else
        return NE;
    #endif
}
{LE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LE");
    #else
        return LE;
    #endif
}
{GE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GE");
    #else
        return GE;
    #endif
}
{GT} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GT");
    #else
        return GT;
    #endif
}
{LT} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LT");
    #else
        return LT;
    #endif
}


{ASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ASSIGN");
    #else
        return ASSIGN;
    #endif
}

{SEMICOLON} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SEMICOLON");
    #else
        return SEMICOLON;
    #endif
}

{LPAREN} { 
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LPAREN");
    #else
        return LPAREN;
    #endif
}
{RPAREN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RPAREN");
    #else
    return RPAREN;
    #endif
}
{LBRACE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACE");
    #else
        return LBRACE;
    #endif
}
{RBRACE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACE");
    #else
        return RBRACE;
    #endif
}
{LBRACK} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACK");
    #else
        return LBRACK;
    #endif 
}
{RBRACK} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACK");
    #else
        return RBRACK;
    #endif 
}

{IF} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("IF");
    #else
        return IF;
    #endif
}
{ELSE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ELSE");
    #else
        return ELSE;
    #endif
}
{WHILE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("WHILE");
    #else
        return WHILE;
    #endif
}
{RETURN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RETURN");
    #else
        return RETURN;
    #endif
}
{BREAK} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("BREAK");
    #else
        return BREAK;
    #endif
}
{CONTINUE} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("CONTINUE");
    #else
        return CONTINUE;
    #endif
}

{ADD} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ADD");
    #else
        
        return ADD;
    #endif
}
{SUB} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SUB");
    #else
        return SUB;
    #endif
}
{MUL} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MUL");
    #else
        return MUL;
    #endif
}
{DIV} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("DIV");
    #else
        return DIV;
    #endif
}
{MOD} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MOD");
    #else
        return MOD;
    #endif
}

{AND} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("AND");
    #else
        return AND;
    #endif
}
{OR} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("OR");
    #else
        return OR;
    #endif
}
{NOT} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NOT");
    #else
        return NOT;
    #endif
}

{EOL} yylineno++;
{WHITE}
{ID} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4(yytext);
    #else
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    return ID;
    #endif
}

{DECIMIAL} {
    #ifdef ONLY_FOR_LEX
        fprintf(yyout, "%-12d%-12s%-12s\n",(yylineno+1)/2 , "DECIMIAL", yytext);
    #else
        int val=0;
        for(unsigned int i=0;i<strlen(yytext);i++){
            val=val*10+yytext[i]-'0';
        }
        yylval.itype=val;
        return INTEGER;
    #endif
}
{HEXNUMBER} {
    #ifdef ONLY_FOR_LEX
        int val=0;
        for(unsigned int i=2;i<strlen(yytext);i++){
            val=val*16+yytext[i]-'0';
        }
        fprintf(yyout, "%-12d%-12s%-12d\n", (yylineno+1)/2 , "HEXNUMBER", val);
    #else
        int val=0;
        for(unsigned int i=2;i<strlen(yytext);i++){
            if(yytext[i]>='0'&& yytext[i]<='9'){
                val=val*16+yytext[i]-'0';
            }
            else if(yytext[i]>='a'&&yytext[i]<='z'){
                val=val*16+yytext[i]-'a'+10;
            }
            else if(yytext[i]>='A'&&yytext[i]<='Z'){
                val=val*16+yytext[i]-'A'+10;
            }
            else{
                continue;
            }
        }
        yylval.itype=val;
        return INTEGER;
    #endif
}
{OCTNUMBER} {
    #ifdef ONLY_FOR_LEX
        int val=0;
        for(unsigned int i=1;i<strlen(yytext);i++){
            val=val*8+yytext[i]-'0';
        }

        fprintf(yyout, "%-12d%-12s%-12d\n",(yylineno+1)/2 , "OCTNUMBER", val);
    #else
        int val=0;
        for(unsigned int i=1;i<strlen(yytext);i++){
            val=val*8+yytext[i]-'0';
        }
        yylval.itype=val;
        return INTEGER;
    #endif
}


{commentbegin} {BEGIN COMMENT;}
<COMMENT>{EOL} {yylineno++;}
<COMMENT>{commentelement} {}
<COMMENT>{commentend} {BEGIN INITIAL;}

{commentlinebegin} {BEGIN COMMENTLINE;}
<COMMENTLINE>{commentlineelement} {}
<COMMENTLINE>{commentlineend} {BEGIN INITIAL; yylineno++;}


%%
