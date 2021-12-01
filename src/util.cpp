// #include <iostream>
// #include <stdlib.h>
// #include <ctype.h>
// #include <string.h>
// #include "util.hpp"

// TreeNode *newStmtNode(StmtKind kind)
// {
//     TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
//     int i;
//     if (t == NULL)
//         std::cout << "Out of memory error at line " << lineno << std::endl;
//     else
//     {
//         for (i = 0; i < MAXCHILDREN; i++)
//             t->child[i] = NULL;
//         t->sibling = NULL;
//         t->nodekind = StmtK;
//         t->kind.stmt = kind;
//         t->lineno = lineno;
//     }
//     return t;
// }

// TreeNode *newExpNode(ExpKind kind)
// {
//     TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
//     int i;
//     if (t == NULL)
//         std::cout << "Out of memory error at line " << lineno << std::endl;
//     else
//     {
//         for (i = 0; i < MAXCHILDREN; i++)
//             t->child[i] = NULL;
//         t->sibling = NULL;
//         t->nodekind = ExpK;
//         t->kind.exp = kind;
//         t->lineno = lineno;
//         t->type = Void;
//     }
//     return t;
// }

// static int indentno = 0;

// #define INDENT indentno += 2
// #define UNINDENT indentno -= 2

// static void printSpaces(void)
// {
//     int i;
//     for (i = 0; i < indentno; i++)
//         std::cout << " ";
// }

// void printTree(TreeNode *tree)
// {
//     int i;
//     INDENT;
//     while (tree != NULL)
//     {
//         printSpaces();
//         if (tree->nodekind == StmtK)
//         {
//             switch (tree->kind.stmt)
//             {
//             case IfK:
//                 std::cout << "If" << std::endl;
//                 break;
//             case RepeatK:
//                 std::cout << "Repeat" << std::endl;
//                 break;
//             case AssignK:
//                 std::cout << "Assign to: " << tree->attr.name << std::endl;
//                 break;
//             default:
//                 std::cout << "Unknown ExpNode kind" << std::endl;
//                 break;
//             }
//         }
//         else if (tree->nodekind == ExpK)
//         {
//             switch (tree->kind.exp)
//             {
//             case OpK:
//                 std::cout << "Op: " << tree->attr.op << std::endl;
//                 break;
//             case ConstK:
//                 std::cout << "Const: " << tree->attr.val << std::endl;
//                 break;
//             case IdK:
//                 std::cout << "Id: " << tree->attr.name << std::endl;
//                 break;
//             default:
//                 std::cout << "Unknown ExpNode kind" << std::endl;
//                 break;
//             }
//         }
//         else
//             std::cout << "Unknown node kind" << std::endl;
//         for (i = 0; i < MAXCHILDREN; i++)
//             printTree(tree->child[i]);
//         tree = tree->sibling;
//     }
//     UNINDENT;
// }
