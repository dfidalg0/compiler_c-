#ifndef _UTIL_H_
#define _UTIL_H_

#include <parse.hpp>

extern int lineno;

TreeNode *newStmtNode(StmtKind);

TreeNode *newExpNode(ExpKind);

char *copyString(char *);

void printTree(TreeNode *);

#endif
