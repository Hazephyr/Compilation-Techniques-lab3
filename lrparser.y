%{

#include <stdio.h>
#include "ast.h"

int yylex(void);
int yyerror(char* s);

%}

%expect 1

%union{
    int iValue;
    char* sValue;
    past pAst;
};

%token <sValue> EOL 258
%token <sValue> INT 259
%token <sValue> STR 260
%token <sValue> VOID 261
%token <sValue> IF 262
%token <sValue> ELSE 263
%token <sValue> WHILE 264
%token <sValue> RETURN 265
%token <sValue> PRINT 266
%token <sValue> SCAN 267
%token <sValue> ASSIGN 268
%token <sValue> CMP 269
%token <sValue> ID_VALUE 270
%token <sValue> S_VALUE 271
%token <iValue> I_VALUE 272

%type <pAst>    NUMBER STRING ID
%type <pAst>    program external_declaration function_definition
%type <pAst>    declaration init_declarator_list init_declarator
%type <pAst>    intstr_list initializer declarator
%type <pAst>    direct_declarator parameter_list parameter
%type <pAst>    type statement compound_statement 
%type <pAst>    begin_scope end_scope statement_list
%type <pAst>    expression_statement selection_statement iteration_statement
%type <pAst>    jump_statement print_statement scan_statement
%type <pAst>    expr assign_expr cmp_expr
%type <pAst>    add_expr mul_expr primary_expr
%type <pAst>    expr_list id_list

%%

program: external_declaration    {showAst($1, 0, "start");}
        | program external_declaration    {showAst($2, 0, "start"); }
        ;


external_declaration: function_definition    {$$ = external_declaration($1);}
                    | declaration    {$$ = external_declaration($1);}
                    ;

function_definition: type declarator compound_statement    {$$ = function_definition($1, $2, $3);}
                   ;

declaration: type init_declarator_list ';'    {$$ = declaration($1, $2);}
           ;

init_declarator_list: init_declarator    {$$ = init_declarator_list($1, NULL);}
                    | init_declarator_list ',' init_declarator    {$$ = init_declarator_list($1, $3);}
                    ;

init_declarator: declarator    {$$ = init_declarator($1, NULL);}
               | declarator '=' add_expr    {$$ = init_declarator($1, $3);}
               | declarator '=' '{' intstr_list '}'    {$$ = init_declarator($1, $4);}
               ;

intstr_list: initializer    {$$ = intstr_list($1, NULL);}
           | intstr_list ',' initializer    {$$ = intstr_list($1, $3);}
           ;

initializer: NUMBER    {$$ = initializer($1);}
           | STRING    {$$ = initializer($1);}
           ;

declarator: direct_declarator    {$$ = declarator($1);}
          ;

direct_declarator: ID    {$$ = direct_declarator($1, NULL, NULL);}
                 | direct_declarator '(' parameter_list ')'    {$$ = direct_declarator(NULL, $1, $3);}
                 | direct_declarator '(' ')'    {$$ = direct_declarator(NULL, $1, NULL);}
                 | ID '[' expr ']'    {$$ = direct_declarator($1, $3, NULL);}
                 | ID '[' ']'    {$$ = direct_declarator($1, NULL, NULL);}
                 ;

parameter_list: parameter    {$$ = parameter_list($1, NULL);}
              | parameter_list ',' parameter    {$$ = parameter_list($1, $3);}
              ;

parameter: type ID    {$$ = parameter($1, $2);}
         ;

type: INT    {$$ = type("int");}
    | STR    {$$ = type("str");}
    | VOID    {$$ = type("void");}
    ;

statement: compound_statement    {$$ = statement($1);}
         | expression_statement    {$$ = statement($1);}
         | selection_statement    {$$ = statement($1);}
         | iteration_statement    {$$ = statement($1);}
         | jump_statement    {$$ = statement($1);}
         | print_statement    {$$ = statement($1);}
         | scan_statement    {$$ = statement($1);}
         | declaration    {$$ = statement($1);}
         ;

compound_statement: begin_scope end_scope    {$$ = compound_statement(NULL);}
                  | begin_scope statement_list end_scope    {$$ = compound_statement($2);}
                  ;

begin_scope: '{'    {$$ = begin_scope();}
        ;

end_scope: '}'    {$$ = end_scope();}
         ;

statement_list: statement    {$$ = statement_list($1, NULL);}
              | statement_list statement    {$$ = statement_list($1, $2);}
              ;

expression_statement: ';'   {$$ = expression_statement(NULL);}
                    | expr ';'    {$$ = expression_statement($1);}
                    ;

selection_statement: IF '(' expr ')' statement    {$$ = selection_statement($3, $5, NULL);}
                   | IF '(' expr ')' statement ELSE statement    {$$ = selection_statement($3, $5, $7);}
                   ;

iteration_statement: WHILE '(' expr ')' statement    {$$ = iteration_statement($3, $5);}
                   ;

jump_statement: RETURN ';'    {$$ = jump_statement(NULL);}
              | RETURN expr ';'    {$$ = jump_statement($2);}
              ;

print_statement: PRINT ';'    {$$ = print_statement(NULL);}
               | PRINT expr_list ';'    {$$ = print_statement($2);}
               ;

scan_statement: SCAN id_list ';'    {$$ = scan_statement($2);}
              ;

expr: assign_expr    {$$ = expr($1);}
    ;

assign_expr: cmp_expr    {$$ = assign_expr(NULL, $1, NULL, NULL);}
           | ID ASSIGN assign_expr    {$$ = assign_expr($2, $1, $3, NULL);}
           | ID '=' assign_expr    {$$ = assign_expr(NULL, $1, $3, NULL);}
           | ID '[' expr ']' '=' assign_expr    {$$ = assign_expr(NULL, $1, $3, $6);}
           ;

cmp_expr: add_expr    {$$ = cmp_expr(NULL, $1, NULL);}
        | cmp_expr CMP add_expr    {$$ = cmp_expr($2, $1, $3);}
        ;

add_expr: mul_expr    {$$ = add_expr(NULL, $1, NULL);}
        | add_expr '+' mul_expr    {$$ = add_expr("+", $1, $3);}
        | add_expr '-' mul_expr    {$$ = add_expr("-", $1, $3);}
        ;

mul_expr: primary_expr    {$$ = mul_expr(NULL, $1, NULL);}
        | mul_expr '*' primary_expr    {$$ = mul_expr("*", $1, $3);}
        | mul_expr '/' primary_expr    {$$ = mul_expr("/", $1, $3);}
        | mul_expr '%' primary_expr    {$$ = mul_expr("%", $1, $3);}
        | '-' primary_expr    {$$ = mul_expr("-", $2, NULL);}
        ;

primary_expr: ID '(' expr_list ')'    {$$ = primary_expr($1, $3);}
            | ID '(' ')'    {$$ = primary_expr($1, NULL);}
            | '(' expr ')'    {$$ = primary_expr($2, NULL);}
            | ID    {$$ = primary_expr($1, NULL);}
            | initializer    {$$ = primary_expr($1, NULL);}
            | ID '[' expr ']'    {$$ = primary_expr($1, $3);}
            ;

expr_list: expr    {$$ = expr_list($1, NULL);}
         | expr_list ',' expr    {$$ = expr_list($1, $3);}
         ;

id_list: ID    {$$ = id_list($1, NULL);}
       | id_list ',' ID    {$$ = id_list($1, $3);}
       ;

NUMBER: I_VALUE    {$$ = astNUMBER($1);}
      ;

STRING: S_VALUE    {$$ = astSTRING($1);}
      ;

ID: ID_VALUE    {$$ = astID($1);}
  ;
   
%%
