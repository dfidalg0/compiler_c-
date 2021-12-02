#ifndef _TREE_H_
#define _TREE_H_

typedef enum {
    Statement,
    Expression
} NodeKind;

typedef enum {
    If,
    While,
    Assign,
    Return,
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
    Integer,
    Boolean
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

#endif
