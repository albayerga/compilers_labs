#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define TRUE 1
#define FALSE 0

//GLOBAL FLAGS
struct structFlags
{
    int showManPage;
    int processComments;
    int processDirectives;
    int processIfDef;
    int processInclude;
    int processAll;
    int printHelp;
};

struct structFlags flags = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

//GLOBAL DICTIONARY
struct KeyValuePair
{
    char key[50];
    char value[50];
};

struct KeyValuePair dictionary[1000];
int dictionarySize = 0;

//TOGGLE FLAGS
void toggleFlags(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-help") == 0)
        {
            flags.showManPage = TRUE;
        }
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
        }
        else if(strcmp(argv[i], "-help") == 0)
        {
            flags.printHelp = TRUE;
        }
    }
}

//FUNCTIONS
void printManPage();
char *takeWord(FILE *inputFile, long *filePosition);
void processFile(FILE *inputFile, FILE *outputFile, long *filePosition, struct structFlags flags);
void processInclude(FILE *inputFile, FILE *outputFile, const char *filename);
void processDefine(FILE *inputFile, FILE *outputFile);
void processIfDef(FILE *inputFile, FILE *outputFile);
void processComments(FILE *inputFile, FILE *outputFile);

void printManPage()
{
    printf("MAN PAGE:\n");
    printf("Usage: preprocessor [flags] <program.c>\n");
    printf("Flags:\n");
    printf("  -c      Eliminate comments\n");
    printf("  -d      Replace all directives starting with #\n");
    printf("  -all    Do all processing (comments and directives)\n");
    printf("  -help   Print this man page\n");
}

//general take word
char *takeWord(FILE *inputFile, long *filePosition)
{
    char *word = malloc(100);
    int c;
    int i = 0;
    while((c = fgetc(inputFile)) != EOF && !isspace(c))
    {
        word[i] = c;
        i++;
    }
    word[i] = '\0';
    *filePosition = ftell(inputFile);
    return word;
}

//this is for include
char *takeFileWord(FILE *inputFile, long *filePosition)
{
    char *word = malloc(100);
    int c;
    int i = 0;

    // Skip leading whitespace
    while ((c = fgetc(inputFile)) != EOF && isspace(c))
    {
        (*filePosition)++;
    }

    // Check if the word is enclosed in double quotes or <>
    if (c == '"' || c == '<')
    {
        int closingChar = (c == '"') ? '"' : '>';
        while ((c = fgetc(inputFile)) != EOF && c != closingChar)
        {
            word[i] = c;
            i++;
            (*filePosition)++;
        }
    }
    else
    {
        // If not enclosed in quotes or <>, read until whitespace or EOF
        while ((c = fgetc(inputFile)) != EOF && !isspace(c))
        {
            word[i] = c;
            i++;
            (*filePosition)++;
        }
    }

    word[i] = '\0';
    return word;
}

/* --------------------------------------------- */

//PROCESS FILE
void processFile(FILE *inputFile, FILE *outputFile, long *filePosition, struct structFlags flags)
{
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

        if ((strcmp(word, "#include") == 0) && (flags.processInclude == TRUE))
        {
            char* includedFileName = takeFileWord(inputFile, filePosition);
            // Process the included file
            processInclude(inputFile, outputFile, includedFileName);
            free(includedFileName);
            
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

    long includedFilePosition = 0;
    processFile(includedFile, outputFile, &includedFilePosition, flags); //recursive call

    fclose(includedFile);
}

/* --------------------------------------------- */

//MAIN
int main(int argc, char *argv[])
{
    argc = 3;
    argv[0] = "./my_preprocessor";
    argv[1] = "-d";
    argv[2] = "comp-p1.c";

    toggleFlags(argc, argv);

    if(argc < 2 )
    {
        printf("Usage: preprocessor [flags] <program.c>\n");
        return 0;
    }
    else if (argc == 2 && flags.printHelp == TRUE)
    {
        printManPage();
        return 0;
    }

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
    //read the file, when you find a key word, if the flag is on, process it
    long filePosition = 0;
    processFile(inputFile, outputFile, &filePosition, flags);

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

