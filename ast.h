#define MAX_STR_LEN 64

typedef struct _ast ast;
typedef struct _ast* past;

typedef union
{
    int ivalue;
    char svalue[MAX_STR_LEN];
} _uvalue;

typedef struct
{
    enum {
        UNUSED,
        IVALUE,
        SVALUE
    } type;
    _uvalue value;
} uvalue;

struct _ast
{
    uvalue uvalue;
    char nodeType[MAX_STR_LEN];
    struct _ast* left;
    struct _ast* right;
    struct _ast* rright;
};

void showAst(past pnode, int nest, char* parentNodeType);
past astNUMBER(int I_VALUE);
past astSTRING(char* S_VALUE);
past astID(char* ID_VALUE);
past newAstNode(char* nodeType, past left, past right);
past program(past left, past right);
past external_declaration(past left);
past function_definition(past left, past right, past rright);
past declaration(past left, past right);
past init_declarator_list(past left, past right);
past init_declarator(past left, past right);
past intstr_list(past left, past right);
past initializer(past left);
past declarator(past left);
past direct_declarator(past left, past right, past rright);
past parameter_list(past left, past right);
past parameter(past left, past right);
past type(char* type);
past statement(past left);
past compound_statement(past left);
past begin_scope();
past end_scope();
past statement_list(past left, past right);
past expression_statement(past left);
past selection_statement(past left, past right, past rright);
past iteration_statement(past left, past right);
past jump_statement(past left);
past print_statement(past left);
past scan_statement(past left);
past expr(past left);
past assign_expr(char* oprand, past left, past right, past rright);
past cmp_expr(char* cmp, past left, past right);
past add_expr(char* oprand, past left, past right);
past mul_expr(char* oprand, past left, past right);
past primary_expr(past left, past right);
past expr_list(past left, past right);
past id_list(past left, past right);