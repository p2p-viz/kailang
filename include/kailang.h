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

#define KAI_AST_NODE_FUNCTION       0x5415
#define KAI_AST_NODE_SCOPE          0x5416
#define KAI_AST_NODE_EXPRESSION     0x5417
#define KAI_AST_NODE_CONSTANT       0x5418

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
    struct KAI_ASTNode* childrenNodes[4096];
    int childrenNodeCount;
    int id;
    int type;
};

struct KAI_Context
{
    int globalId;
};

// functions

struct KAI_ASTNode* KAI_Compile(struct KAI_CMDArgs* args);
void KAI_FreeAstNode(struct KAI_ASTNode* node);

#endif // KAILANG_H