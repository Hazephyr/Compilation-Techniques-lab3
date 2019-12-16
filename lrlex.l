%{
#include <stdlib.h>
#include "ast.h"
#include "lrparser.tab.h"


int yyerror(char* s);
%}

DIGIT [0-9]
LETTER [a-zA-Z]
STRING \"[^\"]*\"

%%

string                              {return STR;}
int                                 {return INT;}
str                                 {return STR;}
void                                {return VOID;}
if                                  {return IF;};
else                                {return ELSE;}
while                               {return WHILE;}
return                              {return RETURN;}
print                               {return PRINT;}
scan                                {return SCAN;}
<<EOF>>                             {return EOF;}

{DIGIT}+                            {yylval.iValue = atoi(yytext); return I_VALUE;}
[ |\t]                              { /* eat up blank space */ }
\n                                  { /* line++; */ }
"="|"+"|"-"|"*"|"/"|"%"             {return yytext[0];}
";"|","|"("|")"|"["|"]"|"{"|"}"     {return yytext[0];}
"=="|">"|"<"|"<="|">="              {yylval.sValue = yytext; return CMP;}
"+="|"-="|"*="|"/="|"%="            {yylval.sValue = yytext; return ASSIGN;}
{LETTER}({LETTER}|{DIGIT})*         {yylval.sValue = yytext; return ID_VALUE;}
{STRING}                            {yylval.sValue = yytext; return S_VALUE;}
("//".*)|("/*"([^*]|"*"[^/])*"*/")  { /* eat up comments */ }

%%

int yyerror(char* s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}
