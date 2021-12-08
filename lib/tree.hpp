#ifndef _TREE_H_
#define _TREE_H_

#include <string>

typedef enum {
    Statement,
    Expression
} NodeKind;

typedef enum {
    If,
    While,
    Assign,
    Return,
    VariableDeclaration,
    StatementList
} StatementKind;

typedef enum {
    Operation,
    Constant,
    Identifier,
    Array,
    Function,
    FunctionCall,
    ParamsList
} ExpressionKind;

typedef enum {
    Void,
    Integer
} ExpressionType;

struct TreeNode {
    TreeNode *child[3];
    TreeNode *sibling;
    int line;
    NodeKind nodekind;

    union {
        StatementKind statement;
        ExpressionKind expression;
    } kind;

    union {
        int operation;
        int val;
        int pos;
        char * name;
    } attr;

    ExpressionType type;
};

TreeNode * createStatementNode(StatementKind, int line);

TreeNode * createExpressionNode(ExpressionKind, int line);

void printTree(TreeNode *);

char *copyString(std::string s);

std::string getOperationType(int op);

#endif
