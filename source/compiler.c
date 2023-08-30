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

void KAI_PrintAST(struct KAI_ASTNode* node, int depth)
{
    if(node == NULL)
    {
        return;
    }
    for (int i = 0 ; i < depth ; i++)
    {
        printf("  ");
    }
    printf("Node %d: | %s | %d\n", node->id, node->name, node->type);
    for (int  i = 0 ; i < node->childrenNodeCount ; i++)
    {
        KAI_PrintAST(node->childrenNodes[i], depth + 1);
    }
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
    sprintf(node->name, "scope_%d", node->id);
    return node;
}

struct KAI_ASTNode* KAI_CreateNamedNode(struct KAI_Context* globalContext, const char* name, int tokenSize)
{
    struct KAI_ASTNode* node = (struct KAI_ASTNode*)malloc(sizeof(struct KAI_ASTNode));
    node->childrenNodeCount = 0;
    node->id = globalContext->globalId++;
    node->type = KAI_AST_NODE_FUNCTION;
    strncpy(node->name, name, tokenSize);
    node->name[tokenSize] = 0;
    return node;
}

bool KAI_IsSeparator(char ch)
{
    return (ch == '.' ||
            ch == ',' ||
            ch == '(' ||
            ch == ')' ||
            ch == '{' ||
            ch == '}' ||
            ch == '[' ||
            ch == ']' ||
            ch == '<' ||
            ch == '>' ||
            ch == '|' ||
            ch == '^' ||
            ch == '-' ||
            ch == ':');
}

bool KAI_IsOperator(char ch)
{
    return (ch == '+' ||
            ch == '-' ||
            ch == '*' ||
            ch == '/' ||
            ch == '%' ||
            ch == '=' ||
            ch == '&' ||
            ch == '|' ||
            ch == '<' ||
            ch == '>' ||
            ch == '^' ||
            ch == '!');
}

int KAI_DetectTokenType(char ch)
{
    int tokenType = KAI_TOKEN_TYPE_IDENTIFIER;
    if(isalpha(ch)) 
    {
        tokenType = KAI_TOKEN_TYPE_IDENTIFIER;
    }
    else if (isdigit(ch))
    {
        tokenType = KAI_TOKEN_TYPE_INT;
    }
    else if (ch == '\"')
    {
        tokenType = KAI_TOKEN_TYPE_STRING;
    }
    else if (KAI_IsSeparator(ch))
    {
        tokenType = KAI_TOKEN_TYPE_SEPERATOR;
    }
    else if (KAI_IsOperator(ch))
    {
        tokenType = KAI_TOKEN_TYPE_OPERATOR;
    }
    else if (isspace(ch))
    {
        tokenType = KAI_TOKEN_TYPE_WHITESPACE;
    }
    else 
    {
        tokenType = KAI_TOKEN_TYPE_IDENTIFIER;
    }
    return tokenType;
}

const char* KAI_SkipWhitespace(const char* startPtr, const char* endPtr, bool stopAtLineFeed)
{
    while(startPtr != endPtr)
    {
        if(!isspace(*startPtr))
        {
            break;
        }

        if(stopAtLineFeed && *startPtr == '\n' && (  (startPtr + 1) == endPtr || *(startPtr + 1) != '\n'))
        {
            break;
        }

        startPtr++;
    }
    return startPtr;
}

const char* KAI_NextToken(const char* startPtr, const char* endPtr, const char** tokenPtr, int* tokenSize, int* tokenType)
{
    const char* currPtr = (const char*)startPtr;
    int currTokenType = KAI_DetectTokenType(*currPtr), nextTokenType = 0;
    int currTokenSize = 1;
    bool isInString = currTokenType == KAI_TOKEN_TYPE_STRING;
    
    currPtr++;
    while (currPtr != endPtr)
    {
        if (isInString)
        {
            // esacpe sequence
            if (*currPtr == '\"' && *(currPtr - 1) != '\\')
            {
                isInString = false;
                currPtr++;
                currTokenSize++;
                break;
            }
        }
        else if (currTokenType != (nextTokenType = KAI_DetectTokenType(*currPtr)))
        {
            if( (currTokenType == KAI_TOKEN_TYPE_INT && *currPtr == '.') 
             || (currTokenType == KAI_TOKEN_TYPE_FLOAT && nextTokenType == KAI_TOKEN_TYPE_INT) )
            {
                currTokenType = KAI_TOKEN_TYPE_FLOAT;
            }
            else if(currTokenType == KAI_TOKEN_TYPE_IDENTIFIER && nextTokenType == KAI_TOKEN_TYPE_INT)
            {
                currTokenType = KAI_TOKEN_TYPE_IDENTIFIER;
            }
            else 
            {
                break;
            }            
        }
        currPtr++;
        currTokenSize++;
    }

    if(tokenSize) 
    {
        *tokenSize = currTokenSize;
    }

    if(tokenPtr)
    {
        *tokenPtr = startPtr;
    }

    if(tokenType)
    {
        *tokenType = currTokenType;
    }

    return currPtr;
}

const char* KAI_ParseExpression(const char* startPtr, const char* endPtr, struct KAI_ASTNode* node, struct KAI_Context* globalContext)
{
    if (endPtr <= startPtr)
    {
        return false;
    }

    const char* tokenPtr = NULL;
    int tokenSize = 0;
    int tokenType = 0;
    bool isInBraces = false;

    startPtr = KAI_SkipWhitespace(startPtr, endPtr, false);
    node->type = KAI_AST_NODE_EXPRESSION;


    while((startPtr = KAI_NextToken(startPtr, endPtr, &tokenPtr, &tokenSize, &tokenType)) != endPtr)
    {
        if ((*tokenPtr == '\n' && !isInBraces)
         || *tokenPtr == '}' 
         || (globalContext->brackEscape > 0 && *tokenPtr == ')')
         || (globalContext->angleEscape > 0 && *tokenPtr == '>')) 
        {
            globalContext->brackEscape = globalContext->brackEscape > 0 ? globalContext->brackEscape - 1 : 0;
            globalContext->angleEscape = globalContext->angleEscape > 0 ? globalContext->angleEscape - 1 : 0;
            break;
        }
        else if (*tokenPtr == '{')
        {
            isInBraces = true;
        }
        else if (*tokenPtr == '(')
        {
            globalContext->brackEscape += 1;

            struct KAI_ASTNode* expressionNode = KAI_CreateScopeNode(globalContext);
            node->childrenNodes[node->childrenNodeCount++] = expressionNode;
            if((startPtr = KAI_ParseExpression(startPtr, endPtr, expressionNode, globalContext)) == endPtr)
            {
                return startPtr;
            }
        }
        else if (*tokenPtr == '<')
        {
            globalContext->brackEscape += 1;

            struct KAI_ASTNode* expressionNode = KAI_CreateScopeNode(globalContext);
            node->childrenNodes[node->childrenNodeCount++] = expressionNode;
            if((startPtr = KAI_ParseExpression(startPtr, endPtr, expressionNode, globalContext)) == endPtr)
            {
                return startPtr;
            }
        }
        else if (strncmp(tokenPtr, "let", tokenSize) == 0)
        {
            startPtr = KAI_SkipWhitespace(startPtr, endPtr, false);
            startPtr = KAI_NextToken(startPtr, endPtr, &tokenPtr, &tokenSize, &tokenType);

            if (tokenType != KAI_TOKEN_TYPE_IDENTIFIER)
            {
                printf("Expected identifier after let\n");
                return false;
            }
            struct KAI_ASTNode* letNode = KAI_CreateNamedNode(globalContext, tokenPtr, tokenSize);            
            letNode->type = KAI_AST_NODE_LET;
            node->childrenNodes[node->childrenNodeCount++] = letNode;
            startPtr = KAI_SkipWhitespace(startPtr, endPtr, false);
            startPtr = KAI_NextToken(startPtr, endPtr, &tokenPtr, &tokenSize, &tokenType);
            if (*tokenPtr != '=')
            {
                printf("Expected = after let\n");
                return false;
            }
            startPtr = KAI_SkipWhitespace(startPtr + 1, endPtr, false);
            struct KAI_ASTNode* expressionNode = KAI_CreateScopeNode(globalContext);
            letNode->childrenNodes[letNode->childrenNodeCount++] = expressionNode;
            if((startPtr = KAI_ParseExpression(startPtr, endPtr, expressionNode, globalContext)) == endPtr)
            {
                return endPtr;
            }
            
            if(!isInBraces) 
            {
                return startPtr;
            }
        }
        else if(strncmp("->", tokenPtr, 2) == 0)
        {
            struct KAI_ASTNode* expressionNode = KAI_CreateScopeNode(globalContext);
            node->childrenNodes[node->childrenNodeCount++] = expressionNode;
            if((startPtr = KAI_ParseExpression(startPtr, endPtr, expressionNode, globalContext)) == endPtr)
            {
                return startPtr;
            }
            if(!isInBraces) 
            {
                return startPtr;
            }
        }
        else if (tokenType == KAI_TOKEN_TYPE_IDENTIFIER)
        {
            struct KAI_ASTNode* idenNode = KAI_CreateNamedNode(globalContext, tokenPtr, tokenSize);            
            idenNode->type = KAI_AST_NODE_IDENTIFIER;
            node->childrenNodes[node->childrenNodeCount++] = idenNode;
        }
        else if (tokenType == KAI_TOKEN_TYPE_INT || tokenType == KAI_TOKEN_TYPE_FLOAT || tokenType == KAI_TOKEN_TYPE_STRING)
        {
            struct KAI_ASTNode* idenNode = KAI_CreateNamedNode(globalContext, tokenPtr, tokenSize);            
            idenNode->type = KAI_AST_NODE_LITERAL;
            node->childrenNodes[node->childrenNodeCount++] = idenNode;
        }
        else if (tokenType == KAI_TOKEN_TYPE_OPERATOR || tokenType == KAI_TOKEN_TYPE_SEPERATOR)
        {
            struct KAI_ASTNode* idenNode = KAI_CreateNamedNode(globalContext, tokenPtr, tokenSize);            
            idenNode->type = KAI_AST_NODE_OPERATOR;
            node->childrenNodes[node->childrenNodeCount++] = idenNode;
        }
        
        startPtr = KAI_SkipWhitespace(startPtr, endPtr, true);
    }
    return startPtr;
}

struct KAI_ASTNode* KAI_Compile(struct KAI_CMDArgs* args)
{
    bool verbose = args->verbose;
    struct KAI_Context globalContext = {0};
    KAI_InitContext(&globalContext);

    globalContext.brackEscape = 0;
    globalContext.angleEscape = 0;

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

    const char* startPtr = source, *endPtr = source + strlen(source);

    while((startPtr = KAI_ParseExpression(startPtr, endPtr, root, &globalContext)) < endPtr)
    {
    }

    KAI_VERBOSE_PRINT("File %s parsed successfully\n", args->sourceFile);


    if (verbose)
    {
        printf("AST:\n");
        KAI_PrintAST(root, 0);
    }

    free(source);

    return root;
}
