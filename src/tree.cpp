#include <iostream>
#include <string.h>
#include <tree.hpp>
#include <vector>
#include <parser.yy.hpp>

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

static int indentNo = -2;

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

std::string getOperationType(int op) {
    static const std::string types[] = {
        "==",
        "!=",
        "<",
        "<=",
        ">",
        ">=",
        "+",
        "-",
        "*",
        "/",
    };

    return types[op - yytokentype::EQ];
}

void printTree(TreeNode * tree, std::string label) {
    INDENT;
    printSpaces();
    std::cout << label << std::endl;
    printTree(tree);
    UNINDENT;
}

void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while (tree != NULL) {
        std::vector<std::string> labels;

        if (tree->nodekind == Statement) {
            if (tree->kind.statement != StatementList) {
                printSpaces();
            }

            switch (tree->kind.statement)
            {
            case If:
                std::cout << "If" << std::endl;
                labels.push_back("- Condition:");
                labels.push_back("- Body:");
                break;
            case While:
                std::cout << "While" << std::endl;
                labels.push_back("- Condition:");
                labels.push_back("- Body:");
                break;
            case Assign:
                std::cout << "Assignment" << std::endl;
                labels.push_back("- Variable");
                labels.push_back("- Value:");
                break;
            case Return:
                std::cout << "Return statement";
                if (tree->child[0]) {
                    labels.push_back("- Value:");
                }
                else {
                    std::cout << " (void)";
                }
                std::cout << std::endl;
                break;
            case VariableDeclaration:
                std::cout << "Variable declaration (Type: " << returnExpressionType(tree->type) << ")" << std::endl;
                labels.push_back("- Variable:");
                break;
            case StatementList:
                if (!tree->sibling) {
                    printSpaces();
                    std::cout << "Empty scope" << std::endl;
                }
                break;
            default:
                std::cout << "Unknown Statement Node Kind: " << tree->kind.statement << std::endl;
                break;
            }
        }
        else if (tree->nodekind == Expression) {
            if (tree->kind.expression != ParamsList) {
                printSpaces();
            }

            switch (tree->kind.expression) {
            case Operation:
                std::cout << "Operation: " << getOperationType(tree->attr.operation) << std::endl;
                break;
            case Constant:
                std::cout << "Constant: " << tree->attr.val << std::endl;
                break;
            case Identifier:
                std::cout << "Id: " << tree->attr.name << std::endl;
                break;
            case Array:
                std::cout << "Array: " << std::endl;
                labels.push_back("- Identifier:");
                if (tree->child[0]) {
                    labels.push_back("- Indexer:");
                }
                break;
            case Function:
                std::cout << "Function: " << tree->attr.name << ", Type: " << returnExpressionType(tree->type) << std::endl;
                labels.push_back("- Params:");
                labels.push_back("- Body:");
                break;
            case FunctionCall:
                std::cout << "FunctionCall: " << tree->attr.name << std::endl;
                labels.push_back("- Params:");
                break;
            case ParamsList:
                if (!tree->sibling) {
                    printSpaces();
                    std::cout << "No params" << std::endl;
                }
                break;
            default:
                std::cout << "Unknown Expression Node Kind: " << tree->kind.expression << std::endl;
                break;
            }
        }
        else
            std::cout << "Unknown Node Kind" << std::endl;
        for (i = 0; i < MAXCHILDREN; i++) {
            if (i < labels.size()) {
                printTree(tree->child[i], labels[i]);
            }
            else {
                printTree(tree->child[i]);
            }
        }
        labels.clear();
        tree = tree->sibling;
    }
    UNINDENT;
}
