%option noyywrap
%option yylineno
%{
    /*
    * You will need to comment this line in lab5.
    */
    #define ONLY_FOR_LEX
    
    #ifdef ONLY_FOR_LEX
    #else
    #define YYSTYPE int
    #include "parser.h"
    #endif

    #define YY_NO_UNPUT
    #define YY_NO_INPUT
    #include <string>
    #include <stdio.h>

    #ifdef ONLY_FOR_LEX
    #include <iostream>
    #include <iomanip>
    #include <fstream>
    #include <math.h>
    using namespace std;
    extern FILE *yyin; 
    extern FILE *yyout;

    void DEBUG_FOR_LAB4(std::string s){
        //std::string DEBUG_INFO = "[DEBUG LAB4]: \t" + s1 + "\n";
        //fputs(DEBUG_INFO.c_str(), yyout);
        fprintf(yyout, "%-12d%-12s%-12s\n", (yylineno+1)/2 , s.c_str(), yytext);
    }
    #endif
%}
%{
	int id_num = 0;
	char id_list[100][20];
%}

VOID "void"
INT "int"
CONST "const"

SEMICOLON ";"

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
LE <=
GE >=
EQL ==
GT >
LT <

AND "&&"
OR "||"
NOT "!"

ASSIGN "="
ADDASSIGN "+="
SUBASSIGN "-="
MULASSIGN "*="
DIVASSIGN "/="
MODASSIGN "%="

ADD "+"
MINUS "-"
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
%x BLOCKCOMMENT

%%
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

{EQL} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("EQL");
    #else
        return EQL;
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
{ADDASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ADDASSIGN");
    #else
        return ADDASSIGN;
    #endif
}
{SUBASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SUBASSIGN");
    #else
        return SUBASSIGN;
    #endif
}
{MULASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MULASSIGN");
    #else
        return MULASSIGN;
    #endif
}
{DIVASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("DIVASSIGN");
    #else
        return DIVASSIGN;
    #endif
}
{MODASSIGN} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MODASSIGN");
    #else
        return MODASSIGN;
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
{MINUS} {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MINUS");
    #else
        return MINUS;
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
	bool id_match = 0;
	int id = 0;
	for (int i = 0; i < id_num; i++) {
		if (strcmp(yytext, id_list[i])==0) {
			id_match = 1;
			id = i;
			break;
		}
	}
	if (!id_match) {
		id = id_num;
		strcpy(id_list[id], yytext);
		id_num++;
	}
	fprintf(yyout, "%-12d%-12s%-12s%-12d\n", (yylineno+1)/2 ,"ID", yytext , id);
    #else
        return ID;
    #endif
}

{DECIMIAL} {
    #ifdef ONLY_FOR_LEX
        //yylval=atoi(yytext);
        //fputs("[DEBUG LAB4]: \t", yyout);
        //fputs("DECIMIAL\t",yyout);
        //fputs(yytext,yyout);
        //fputs("\n",yyout);
        fprintf(yyout, "%-12d%-12s%-12s\n",(yylineno+1)/2 , "DECIMIAL", yytext);
    #else
        //yylval=atoi(yytext);
        return DECIMIAL;
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
        //yylval=atoi(yytext);
        return HEXNUMBER;
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
        //yylval=atoi(yytext);
        return OCTNUMBER;
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

#ifdef ONLY_FOR_LEX
int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "Argument Not Enough");
        exit(EXIT_FAILURE);
    }

    if(!(yyin = fopen(argv[1], "r"))){
        fprintf(stderr, "No such file or directory: %s", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(!(yyout = fopen(argv[3], "w"))){
        fprintf(stderr, "No such file or directory: %s", argv[3]);
        exit(EXIT_FAILURE);
    }
    fprintf(yyout, "%-12s%-12s%-12s%-12s\n","LINENUMBER" , "token", "lexeme" , "IDTNUM");
    yylex();
    return 0;
}
#endif