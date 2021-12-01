
#ifndef _PARSE_H_
#define _PARSE_H_

#include "../../lexer/lib/lexer.h"
#include "../../main.tab.h"

#define MAXCHILDREN 3

typedef enum
{
  Void,
  Integer,
  Boolean
} ExpType;

typedef enum
{
  StmtK,
  ExpK
} NodeKind;

typedef enum
{
  IfK,
  RepeatK,
  AssignK,
} StmtKind;

typedef enum
{
  OpK,
  ConstK,
  IdK
} ExpKind;

typedef struct treeNode
{
  struct treeNode *child[MAXCHILDREN];
  struct treeNode *sibling;
  int lineno;
  NodeKind nodekind;
  union
  {
    StmtKind stmt;
    ExpKind exp;
  } kind;
  union
  {
    yytokentype op;
    int val;
    char *name;
  } attr;
  ExpType type;
} TreeNode;

TreeNode *parse(void);

#endif