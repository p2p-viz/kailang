#include "kailang.h"

void KAI_FreeAstNode(struct KAI_ASTNode* node)
{
    if(node == NULL)
    {
        return;
    }
    
    for (int  i = 0 ; i < node->childrenNodeCount ; i++)
    {
        KAI_FreeAstNode(node->childrenNodes[i]);
    }
    free(node);
}

char* KAI_ReadFile(const char* file)
{
    FILE* f = fopen(file, "rb");
    if (f == NULL)
    {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    
    string[fsize] = 0;
    return string;
}

void KAI_InitContext(struct KAI_Context* context)
{
    context->globalId = 0;
}

struct KAI_ASTNode* KAI_CreateScopeNode(struct KAI_Context* globalContext)
{
    struct KAI_ASTNode* node = (struct KAI_ASTNode*)malloc(sizeof(struct KAI_ASTNode));
    node->childrenNodeCount = 0;
    node->id = globalContext->globalId++;
    node->type = KAI_AST_NODE_SCOPE;
    return node;
}

bool KAI_Parse(const char* startPtr, const char* endPtr, struct KAI_ASTNode* node, struct KAI_Context* globalContext)
{
    return true;
}

struct KAI_ASTNode* KAI_Compile(struct KAI_CMDArgs* args)
{
    bool verbose = args->verbose;
    struct KAI_Context globalContext = {0};
    KAI_InitContext(&globalContext);

    KAI_VERBOSE_PRINT("Reading file %s\n", args->sourceFile);
    char* source = KAI_ReadFile(args->sourceFile);

    if (source == NULL)
    {
        KAI_VERBOSE_PRINT("Failed to read file %s\n", args->sourceFile);
        return NULL;
    }

    KAI_VERBOSE_PRINT("File %s read successfully\n", args->sourceFile);

    KAI_VERBOSE_PRINT("Parsing file %s\n", args->sourceFile);
    struct KAI_ASTNode* root = KAI_CreateScopeNode(&globalContext);

    if(!KAI_Parse(source, source + strlen(source), root, &globalContext))
    {
        KAI_FreeAstNode(root);
        root = NULL;
        KAI_VERBOSE_PRINT("Failed to parse file %s\n", args->sourceFile);
    }

    KAI_VERBOSE_PRINT("File %s parsed successfully\n", args->sourceFile);

    free(source);

    return root;
}
