#ifndef KAILANG_H
#define KAILANG_H

#define KAI_VERSION_MAJOR 0
#define KAI_VERSION_MINOR 0
#define KAI_VERSION_PATCH 1

// std includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// macros

#define KAI_AST_NODE_LET            1
#define KAI_AST_NODE_FUNCTION       2
#define KAI_AST_NODE_SCOPE          3
#define KAI_AST_NODE_EXPRESSION     4
#define KAI_AST_NODE_CONSTANT       5
#define KAI_AST_NODE_IDENTIFIER     6
#define KAI_AST_NODE_LITERAL        7
#define KAI_AST_NODE_OPERATOR       8

#define KAI_TOKEN_TYPE_FLOAT         6
#define KAI_TOKEN_TYPE_STRING        7
#define KAI_TOKEN_TYPE_IDENTIFIER    8
#define KAI_TOKEN_TYPE_OPERATOR      9
#define KAI_TOKEN_TYPE_SEPERATOR     10
#define KAI_TOKEN_TYPE_WHITESPACE    11
#define KAI_TOKEN_TYPE_UNKNOWN       12
#define KAI_TOKEN_TYPE_INT           13

#define KAI_VERBOSE_PRINT(...) if(verbose) { printf("VERBOSE: "); printf(__VA_ARGS__); }

// structs 


struct KAI_CMDArgs
{
    const char* sourceFile;
    const char* outputFile;
    const char* includePaths[256];
    int includePathCount;
    bool verbose;
};

struct KAI_ASTNode
{
    char name[1024];
    struct KAI_ASTNode* childrenNodes[4096];
    int childrenNodeCount;
    int id;
    int type;
};

struct KAI_Context
{
    int globalId;
    int brackEscape;
    int angleEscape;
};

// functions

struct KAI_ASTNode* KAI_Compile(struct KAI_CMDArgs* args);
void KAI_FreeAstNode(struct KAI_ASTNode* node);

#endif // KAILANG_H