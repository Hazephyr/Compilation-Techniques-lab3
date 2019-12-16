#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

void showAst(past pnode, int nest, char* parentNodeType){
    if(pnode != NULL)
    {
        if((strcmp(pnode->nodeType, parentNodeType)==0)
            &&!(strcmp(pnode->nodeType, "statement")==0)
            &&!(strcmp(pnode->nodeType, "cmp_expr")==0)
            &&!(strcmp(pnode->nodeType, "add_expr")==0)
            &&!(strcmp(pnode->nodeType, "mul_expr")==0))//打印除表达式的二叉递归节点
        {
            if(pnode->uvalue.type == IVALUE)
                printf(":  %d", pnode->uvalue.value.ivalue);
            else if(pnode->uvalue.type == SVALUE)
                printf(":  %s", pnode->uvalue.value.svalue);
            showAst(pnode->left, nest, pnode->nodeType);
            showAst(pnode->right, nest, pnode->nodeType);
        }
        else
        {
            for(int i = 0; i < nest; i++)
		        printf("  ");
            printf("%s", pnode->nodeType);
            if(pnode->uvalue.type == IVALUE)
                printf(":  %d", pnode->uvalue.value.ivalue);
            else if(pnode->uvalue.type == SVALUE)
                printf(":  %s", pnode->uvalue.value.svalue);
            putchar('\n');
            showAst(pnode->left, nest+1, pnode->nodeType);
            showAst(pnode->right, nest+1, pnode->nodeType);
            showAst(pnode->rright, nest+1, pnode->nodeType);
        }
    }
}

past astNUMBER(int I_VALUE)
{
    past pnode = newAstNode("NUMBER", NULL, NULL);
    pnode->uvalue.value.ivalue = I_VALUE;
    pnode->uvalue.type = IVALUE;
    return pnode;
}

past astID(char* ID_VALUE)
{
    past pnode = newAstNode("ID", NULL, NULL);
    strcpy(pnode->uvalue.value.svalue, ID_VALUE);
    pnode->uvalue.type = SVALUE;
    return pnode;
}

past astSTRING(char* S_VALUE)
{
    past pnode = newAstNode("STRING", NULL, NULL);
    strcpy(pnode->uvalue.value.svalue, S_VALUE);
    pnode->uvalue.type = SVALUE;
    return pnode;
}

past newAstNode(char* nodeType, past left, past right){
    past pnode = (past)malloc(sizeof(ast));
    pnode->left = left;
    pnode->right = right;
    pnode->rright = NULL;
    strcpy(pnode->nodeType, nodeType);
    pnode->uvalue.type = UNUSED;
    
    return pnode;
}

past program(past left, past right){
    past pnode = newAstNode("program", left, right);
    return pnode;
}

past external_declaration(past left){
    past pnode = newAstNode("external_declaration", left, NULL);
    return pnode;
}

past function_definition(past left, past right, past rright){
    past pnode = newAstNode("function_definition", left, right);
    pnode->rright = rright;
    return pnode;
}

past declaration(past left, past right){
    past pnode = newAstNode("declaration", left, right);
    return pnode;
}

past init_declarator_list(past left, past right){
    past pnode = newAstNode("init_declarator_list", left, right);
    return pnode;
}

past init_declarator(past left, past right){
    past pnode = newAstNode("init_declarator", left, right);
    return pnode;
}

past intstr_list(past left, past right){
    past pnode = newAstNode("intstr_list", left, right);
    return pnode;
}

past initializer(past left){
    past pnode = newAstNode("initializer", left, NULL);
    return pnode;
}

past declarator(past left){
    past pnode = newAstNode("declarator", left, NULL);
    return pnode;
}

past direct_declarator(past left, past right, past rright){
    past pnode = newAstNode("direct_declarator", left, right);
    pnode->rright = rright;
    return pnode;
}

past parameter_list(past left, past right){
    past pnode = newAstNode("parameter_list", left, right);
    return pnode;
}

past parameter(past left, past right){
    past pnode = newAstNode("parameter", left, right);
    return pnode;
}

past type(char* type){
    past pnode = newAstNode("type", NULL, NULL);
    strcpy(pnode->uvalue.value.svalue, type);
    pnode->uvalue.type = SVALUE;
    return pnode;
}

past statement(past left){
    past pnode = newAstNode("statement", left, NULL);
    return pnode;
}

past compound_statement(past left){
    past pnode = newAstNode("compound_statement", left, NULL);
    return pnode;
}

past begin_scope(){
    past pnode = newAstNode("begin_scope", NULL, NULL);
    strcpy(pnode->uvalue.value.svalue, "{");
    return pnode;
}

past end_scope(){
    past pnode = newAstNode("end_scope", NULL, NULL);
    strcpy(pnode->uvalue.value.svalue, "}");
    return pnode;
}

past statement_list(past left, past right){
    past pnode = newAstNode("statement_list", left, right);
    return pnode;
}

past expression_statement(past left){
    past pnode = newAstNode("expression_statement", left, NULL);
    return pnode;
}

past selection_statement(past left, past right, past rright){
    past pnode = newAstNode("selection_statement", left, right);
    pnode->rright = rright;
    if(rright == NULL){
        strcpy(pnode->uvalue.value.svalue, "if");
        pnode->uvalue.type = SVALUE;
    }
    else{
        strcpy(pnode->uvalue.value.svalue, "if_else");
        pnode->uvalue.type = SVALUE;
    }
    return pnode;
}

past iteration_statement(past left, past right){
    past pnode = newAstNode("iteration_statement", left, right);
    return pnode;
}

past jump_statement(past left){
    past pnode = newAstNode("iteration_statement", left, NULL);
    return pnode;
}

past print_statement(past left){
    past pnode = newAstNode("print_statement", left, NULL);
    return pnode;
}

past scan_statement(past left){
    past pnode = newAstNode("scan_statement", left, NULL);
    return pnode;
}

past expr(past left){
    past pnode = newAstNode("expr", left, NULL);
    return pnode;
}

past assign_expr(char* oprand, past left, past right, past rright){
    past pnode = newAstNode("assign_expr", left, right);
    if(oprand != NULL){
        strcpy(pnode->uvalue.value.svalue, oprand);
        pnode->uvalue.type = SVALUE;
    }
    pnode->rright = rright;
    return pnode;
}

past cmp_expr( char* cmp, past left, past right){
    past pnode = newAstNode("cmp_expr", left, right);
    if(cmp != NULL){
        strcpy(pnode->uvalue.value.svalue, cmp);
        pnode->uvalue.type = SVALUE;
    }
    return pnode;
}

past add_expr(char* oprand, past left, past right){
    past pnode = newAstNode("add_expr", left, right);
    if(oprand != NULL){
        strcpy(pnode->uvalue.value.svalue, oprand);
        pnode->uvalue.type = SVALUE;
    }
    return pnode;
}

past mul_expr(char* oprand, past left, past right){
    past pnode = newAstNode("mul_expr", left, right);
    if(oprand != NULL){
        strcpy(pnode->uvalue.value.svalue, oprand);
        pnode->uvalue.type = SVALUE;
    }
    return pnode;
}

past primary_expr(past left, past right){
    past pnode = newAstNode("primary_expr", left, right);
    return pnode;
}

past expr_list(past left, past right){
    past pnode = newAstNode("expr_list", left, right);
    return pnode;
}

past id_list(past left, past right){
    past pnode = newAstNode("id_list", left, right);
    return pnode;
}