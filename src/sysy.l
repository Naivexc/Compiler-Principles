%option noyywrap
%option nounput
%option noinput

%{

#include <cstdlib>
#include <string>
// 因为 Flex 会用到 Bison 中关于 token 的定义
// 所以需要 include Bison 生成的头文件
#include "sysy.tab.hpp"
#define debug 0
using namespace std;

%}

/* 空白符和注释 */
WhiteSpace    [ \t\n\r]*
LineComment    "//"[^\n\r]*[\n\r]*
BlockComment "/*"((\*)*[^\*/]|[^\*])*(\*)*"*/"

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+



/*二元运算符*/
MulOperator \*|\/|\%

AddOperator \+|-

/*一元运算符*/
UnaryOperator   !

/*等于无关比较运算符*/
OneLenRelOperator <|>

TwoLenRelOperator <=|>=
/*等于相关比较运算符*/
EqOperator ==|!=

/*逻辑与运算符*/
LAndOperator &&

/*逻辑或运算符*/
LOrOperator \|\|
%%

{WhiteSpace}    {  }
{LineComment}    {  }
{BlockComment}    {  }

"return"        { if(debug)printf("return\n"); return KW_RETURN; }
"const"     { if(debug)printf("const\n"); return KW_CONST;}
"if"    { if(debug)printf("if\n"); return KW_IF;}
"else"    { if(debug)printf("else\n"); return KW_ELSE;}
"while"    { if(debug)printf("while\n"); return KW_WHILE;}  
"break"    { if(debug)printf("break\n"); return KW_BREAK;}
"continue"    { if(debug)printf("continue\n"); return KW_CONTINUE;}
"int"        { if(debug)printf("int\n"); return KW_INT;}
"void"        { if(debug)printf("void\n"); return KW_VOID;}
{Identifier}    { yylval.str_val = new string(std::string("@")+yytext); if(debug)printf("%s\n",yylval.str_val->c_str()); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); if(debug)printf("%d\n",yylval.int_val); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); if(debug)printf("%d\n",yylval.int_val); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); if(debug)printf("%d\n",yylval.int_val); return INT_CONST; }

{MulOperator}     {yylval.one_len_op_val=yytext[0]; if(debug)printf("%c\n",yylval.one_len_op_val); return MUL_OP; }
{AddOperator}     {yylval.one_len_op_val=yytext[0]; if(debug)printf("%c\n",yylval.one_len_op_val); return ADD_OP; }

{UnaryOperator}     { yylval.one_len_op_val=yytext[0]; if(debug)printf("%c\n",yylval.one_len_op_val); return UNARY_OP; }

{TwoLenRelOperator} {yylval.two_len_op_val[0]=yytext[0]; yylval.two_len_op_val[1]=yytext[1]; yylval.two_len_op_val[2]='\0'; 
if(debug)printf("%c\n",yylval.two_len_op_val[0]); if(debug)printf("%c\n",yylval.two_len_op_val[1]); return TWO_LEN_REL_OP;}

{OneLenRelOperator} {yylval.one_len_op_val=yytext[0]; if(debug)printf("%c\n",yylval.one_len_op_val); return ONE_LEN_REL_OP; }
{EqOperator} {yylval.two_len_op_val[0]=yytext[0]; yylval.two_len_op_val[1]=yytext[1]; yylval.two_len_op_val[2]='\0'; 
if(debug)printf("%c\n",yylval.two_len_op_val[0]); if(debug)printf("%c\n",yylval.two_len_op_val[1]);return EQ_OP;}
{LAndOperator} {yylval.two_len_op_val[0]=yytext[0]; yylval.two_len_op_val[1]=yytext[1]; yylval.two_len_op_val[2]='\0'; 
if(debug)printf("%c\n",yylval.two_len_op_val[0]); if(debug)printf("%c\n",yylval.two_len_op_val[1]); return LAND_OP;}
{LOrOperator} {yylval.two_len_op_val[0]=yytext[0]; yylval.two_len_op_val[1]=yytext[1]; yylval.two_len_op_val[2]='\0'; 
if(debug)printf("%c\n",yylval.two_len_op_val[0]); if(debug)printf("%c\n",yylval.two_len_op_val[1]); return LOR_OP;}
.               { char temp=yytext[0]; if(debug)printf("%c\n",temp); return temp; }
%%
