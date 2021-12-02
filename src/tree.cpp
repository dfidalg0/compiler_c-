#include <iostream>
#include <string.h>
#include <tree.hpp>

#define MAXCHILDREN 3

TreeNode *createStatementNode(StatementKind kind, int line)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        std::cout << "Out of memory error at line " << line << std::endl;
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = nullptr;
        t->sibling = nullptr;
        t->nodekind = Statement;
        t->kind.statement = kind;
        t->line = line;
    }
    return t;
}

TreeNode *createExpressionNode(ExpressionKind kind, int line)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        std::cout << "Out of memory error at line " << line << std::endl;
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = nullptr;
        t->sibling = nullptr;
        t->nodekind = Expression;
        t->kind.expression = kind;
        t->line = line;
    }
    return t;
}

char *copyString(std::string s)
{
    char *t = (char*)malloc(s.length() + 1);
    strcpy(t, s.c_str());
    return t;
}

static int indentNo = 0;

#define INDENT indentNo += 2
#define UNINDENT indentNo -= 2

static void printSpaces(void)
{
    int i;
    for (i = 0; i < indentNo; i++)
        std::cout << " ";
}

std::string returnExpressionType(ExpressionType type){
    static const std::string types[] = {
        "void",
        "int",
        "bool"
    };

    return types[type];
}

void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while (tree != NULL)
    {
        printSpaces();
        if (tree->nodekind == Statement)
        {
            switch (tree->kind.statement)
            {
            case If:
                std::cout << "If" << std::endl;
                break;
            case While:
                std::cout << "While" << std::endl;
                break;
            case Assign:
                std::cout << "Assign to: " << tree->attr.name << std::endl;
                break;
            default:
                std::cout << "Unknown Statement Node Kind" << std::endl;
                break;
            }
        }
        else if (tree->nodekind == Expression)
        {
            switch (tree->kind.expression)
            {
            case Operation:
                std::cout << "Operation: " << tree->attr.operation << std::endl;
                break;
            case Constant:
                std::cout << "Constant: " << tree->attr.val << std::endl;
                break;
            case Identifier:
                std::cout << "Id: " << tree->attr.name << ", Type: " << returnExpressionType(tree->type) << std::endl;
                break;
            case Array:
                std::cout << "Array: " << tree->attr.name << ", Type: " << returnExpressionType(tree->type) << std::endl;
                break;
            default:
                std::cout << "Unknown Expression Node Kind" << std::endl;
                break;
            }
        }
        else
            std::cout << "Unknown Node Kind" << std::endl;
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}
