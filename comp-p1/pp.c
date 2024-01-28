#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

//declare functions
void processInclude(FILE *inputFile, FILE *outputFile, const char *filename);
void processFile(FILE *inputFile, FILE *outputFile, long *filePosition);



//GLOBAL FLAGS
struct structFlags
{
    int processComments;
    int processDirectives;
    int processIfDef;
    int processInclude;
    int processAll;
    int printHelp;
};

struct structFlags flags = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

//GLOBAL DICTIONARY
struct KeyValuePair
{
    char key[50];
    char value[50];
};

struct KeyValuePair dictionary[1000];
int dictionarySize = 0;


//DEFINE PUNCTUATION
int myPunct(char c)
{
    if(c == ',' || c== '/' || c == '*' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '.' || c == ':' || c == '?' || c == '!' || c == '-' || c == '+' || c == '%' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '^' || c == '~' || c == '\\' || c == '\'' || c == '\"' || c == '`' || c == '@' || c == '$' || c == '_')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//TOGGLE FLAGS
void toggleFlags(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-c") == 0)
        {
            flags.processComments = TRUE;
        }
        else if(strcmp(argv[i], "-d") == 0)
        {
            flags.processDirectives = TRUE;
            flags.processIfDef = TRUE;
            flags.processInclude = TRUE;
        }
        else if(strcmp(argv[i], "-all") == 0)
        {
            flags.processAll = TRUE;
            flags.processComments = TRUE;
            flags.processDirectives = TRUE;
            flags.processIfDef = TRUE;
            flags.processInclude = TRUE;
        }
        else if(strcmp(argv[i], "-help") == 0)
        {
            flags.printHelp = TRUE;
        }
    }
}

//SPECIAL WORD
int specialWord(char* word)
{
    if(
        (strncmp(word, "//", 2) == 0) ||
        (strncmp(word, "/*", 2) == 0) ||
        (strncmp(word, "#define", 7) == 0) ||
        (strncmp(word, "#include", 8) == 0) ||
        (strncmp(word, "#ifdef", 6) == 0)
    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//TAKE WORD
char *takeWord(FILE *inputFile, long *filePosition)
{
    char *word = NULL;
    int wordSize = 0;
    int c;
    int isSpecialWord = FALSE;

    while (!feof(inputFile))
    {
        c = fgetc(inputFile);
        if (c == EOF)
        {
            break;
        }

        if (isspace(c) || myPunct(c))
        {
            if (wordSize > 0)
            {
                ungetc(c, inputFile);
                break;
            }
            else
            {
                continue;
            }
        }

        if (wordSize == 0)
        {
            word = malloc(1);
        }
        else
        {
            word = realloc(word, wordSize + 1);
        }

        word[wordSize] = c;
        wordSize++;
    }

    if (word != NULL)
    {
        word = realloc(word, wordSize + 1);
        word[wordSize] = '\0';
    }

    return word;
}

/* --------------------------------------------- */
//PROCESS FILE
void processFile(FILE *inputFile, FILE *outputFile, long *filePosition) {
    int copy = TRUE;

    while (!feof(inputFile)) {
        char *word = takeWord(inputFile, filePosition);

        for (int i = 0; i < dictionarySize; i++) {
            if (strcmp(dictionary[i].key, word) == 0) {
                // Replace word with its value from the dictionary
                word = strdup(dictionary[i].value);
                break;
            }
        }

        // Check flags and keywords
        if ((strcmp(word, "#define") == 0) && (flags.processDirectives == TRUE)) {
            // Call processDefine (to be implemented)
            copy = FALSE;
        }

        if ((strcmp(word, "#include") == 0) && (flags.processInclude == TRUE)) {
            word = takeWord(inputFile, filePosition);
            if (word != NULL) {
                // Process the included file
                processInclude(inputFile, outputFile, word);
                free(word);
            }
            copy = FALSE; // Do not copy the #include line to the output
        }

        if ((strcmp(word, "#ifdef") == 0) && (flags.processIfDef == TRUE)) {
            // Call processIfDef (to be implemented)
            copy = FALSE;
        }

        if (((strcmp(word, "//") == 0) || (strcmp(word, "/*"))) && (flags.processComments == TRUE)) {
            // Call processComments (to be implemented)
            copy = FALSE;
        }

        if (copy == TRUE) {
            fprintf(outputFile, "%s", word);
        }

        free(word);
    }
}


//PROCESS INCLUDE
void processInclude(FILE *inputFile, FILE *outputFile, const char *filename)
{
    FILE *includedFile = fopen(filename, "r");
    
    if (includedFile == NULL)
    {
        perror("Error opening included file");
    }

    // Process the content of the included file
    processFile(includedFile, outputFile, NULL);
    fclose(includedFile);
}

/*-----------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    //esto era para hacer debug
    // argc = 3;
    // argv[0] = "./preprocessor_new";
    // argv[1] = "-d";
    // argv[2] = "prueba.c";

    if(argc < 2 )
    {
        printf("Usage: preprocessor [flags] <program.c>\n");
        return 0;
    }
    //else if (-help) -> show man page

    toggleFlags(argc, argv);
    int copy;

    //open input file
    FILE *inputFile = fopen(argv[argc - 1], "r");
    if (!inputFile)
    {
        perror("Error opening input file");
        return 1;
    }

    // Create output file name by adding "_pp" suffix
    char outputFileName[100];
    snprintf(outputFileName, sizeof(outputFileName), "%s_pp.c", argv[argc - 1]);
    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile)
    {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    //process file
    //read the file word by word and process it
    long filePosition = 0;
    processFile(inputFile, outputFile, &filePosition);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}