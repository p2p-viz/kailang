#include "kailang.h"

static void KAI_PrintUsageAndExit(const char* message)
{
    if (message != NULL)
    {
        fprintf(stderr, "Error: %s\n\n", message);
    }
    printf("Usage: kai [options] <source file>\n");
    printf("Options:\n");
    printf("  -o <file>  Specify output file\n");
    printf("  -i <file>  Specify include path\n");
    printf("  -v         Enable verbose output\n");
    exit(message ? EXIT_SUCCESS : EXIT_FAILURE);
}

static bool KAI_ParseCMDArgs(int argc, char** argv, struct KAI_CMDArgs* args)
{
    args->sourceFile = NULL;
    args->outputFile = "a.c";
    args->verbose = false;
    args->includePathCount = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 < argc)
            {
                args->outputFile = argv[++i];
            }
            else
            {
                KAI_PrintUsageAndExit("-o option requires an argument");
            }
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            if (i + 1 < argc)
            {
                args->includePaths[args->includePathCount++] = argv[++i];
            }
            else
            {
                KAI_PrintUsageAndExit("-i option requires an argument");
            }
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            args->verbose = true;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            KAI_PrintUsageAndExit(NULL);
        }
        else if (strcmp(argv[i], "--version") == 0)
        {
            printf("KAI version %d.%d.%d\n", KAI_VERSION_MAJOR, KAI_VERSION_MINOR, KAI_VERSION_PATCH);
            exit(EXIT_SUCCESS);
        }
        else
        {
            if (args->sourceFile == NULL)
            {
                args->sourceFile = argv[i];
            }
            else
            {
                KAI_PrintUsageAndExit("Too many source files");
            }
        }
    }

    if (args->sourceFile == NULL)
    {
        KAI_PrintUsageAndExit("No source file");
    }

    return true;
}

int main(int argc, char** argv, char** enpv)
{
    struct KAI_CMDArgs args = {0};
    if(!KAI_ParseCMDArgs(argc, argv, &args))
    {
        KAI_PrintUsageAndExit("Failed to parse Command Line Arguments");
    }

    struct KAI_ASTNode* compiledCode = KAI_Compile(&args);

    if(!compiledCode)
    {
        KAI_PrintUsageAndExit("Failed to compile code");
    }

    // code gen

    KAI_FreeAstNode(compiledCode);

    return EXIT_SUCCESS;
}