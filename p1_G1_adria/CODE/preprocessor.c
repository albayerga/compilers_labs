#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0




/*-----------------------------------------------------------------------------------*/
//PROGRAM BEHAVIOUR

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

//FILE SCANNER
int takeWord(FILE *file, char *word, int maxLength) {
    int c;
    int i = 0;

    if ((c = fgetc(file)) != EOF && !isalpha(c) && c != '#') {
        word[i++] = c;
        word[i] = '\0';
        return i > 0;

    }

    while (c != EOF && (isalpha(c) || isdigit(c) || c == '#') && i < maxLength - 1) {
        word[i++] = c;
        c = fgetc(file);
    }

    word[i] = '\0';
    if(c != EOF){
        fseek(file, -1L, SEEK_CUR);
    }

    return i > 0;
}

/*-----------------------------------------------------------------------------------*/
//FUNCTION ZONE

//Coments
int removeComments1(FILE *file) {
    int c;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            fseek(file, -1L, SEEK_CUR);
            return 0;
        }
    }
    return 0;
}

int removeComments2(FILE *file) {
    int c;

    while ((c = fgetc(file)) != EOF) {
        if (c == '*') {
            c = fgetc(file);
            if (c == '/') {
                return 1;
            } else {
                ungetc(c, file);
            }
        }
    }
    return 0;
}

//DEFINE

/*
void processDefine(FILE *inputFile)
{
    char word[256];

    takeWord(inputFile, word, 256);

    strcpy(dictionary[dictionarySize].key, word);

    takeWord(inputFile, word, 256);

    if (strcmp(word, '(')) {
        // call HandleMacros
    } else {
        strcpy(dictionary[dictionarySize].value, word);
    }
}
 */


//INCLUDE
//void processFile(FILE * includedFile, FILE * outputFile, struct structFlags flags);

//PROCESS INCLUDE
long processInclude(FILE *inputFile, FILE *outputFile, const char *filename)
{
    FILE *includedFile = fopen(filename, "r");

    if (includedFile == NULL)
    {
        perror("Error opening included file");
        return -1;
    }

    //processFile(includedFile, outputFile, flags); // Recursive call
    fprintf(outputFile, "\n");

    fclose(includedFile);

    return ftell(inputFile);
}


//this is for include
char *takeFileWord(FILE *inputFile)
{
    char *word = malloc(100);
    int c;
    int i = 0;

    // Skip leading whitespace
    while ((c = fgetc(inputFile)) != EOF && isspace(c))
    {
        // No need to increment filePosition here
    }

    // Check if the word is enclosed in double quotes or <>
    if (c == '"' || c == '<')
    {
        int closingChar = (c == '"') ? '"' : '>';
        while ((c = fgetc(inputFile)) != EOF && c != closingChar)
        {
            word[i] = c;
            i++;
            // No need to increment filePosition here
        }
    }
    else
    {
        // If not enclosed in quotes or <>, read until whitespace or EOF
        while ((c = fgetc(inputFile)) != EOF && !isspace(c))
        {
            word[i] = c;
            i++;
            // No need to increment filePosition here
        }
    }

    word[i] = '\0';
    return word;
}

/* --------------------------------------------- */

//PROCESS FILE
void processFile(FILE *inputFile, FILE *outputFile)
{
    int copy;
    char word[50];
    while (!feof(inputFile))
    {
        copy = TRUE;
        takeWord(inputFile, word, sizeof(word));

        // Check flags and keywords
        if ((strcmp(word, "#define") == 0) && (flags.processDirectives == TRUE))
        {
            //processDefine(inputFile);
            printf("Define found\n");
            copy = FALSE;
        }

        if ((strcmp(word, "#include") == 0) && (flags.processInclude == TRUE))
        {
            copy = TRUE; // Do not copy the #include line to the output, now true bacouse function not working!

            //long filePosition = processInclude(inputFile, outputFile, takeFileWord(inputFile));

            //fseek(inputFile, filePosition, SEEK_SET);
        }

        if ((strcmp(word, "#ifdef") == 0) && (flags.processIfDef == TRUE))
        {
            // Call processIfDef (to be implemented)
            copy = FALSE;
        }

        if((strcmp(&word[0],"/") == 0) && (flags.processComments == TRUE))
        {
            int nc = fgetc(inputFile);
            if(nc == '/'){
                removeComments1(inputFile);
                copy = FALSE;

            }else if(nc == '*'){
                removeComments2(inputFile);
                copy = FALSE;
            }

        }

        if (copy == TRUE)
        {
            fprintf(outputFile, "%s", word);
        }
    }
}




/*-----------------------------------------------------------------------------------*/
//MAIN PROGRAM

int main(int argc, char *argv[])
{

    if(argc < 2 )
    {
        printf("Usage: preprocessor [flags] <program.c>\n");
        return 0;
    }

    //Manage flags function call
    toggleFlags(argc, argv);

    //If help flag is toggled
    if(flags.printHelp == TRUE){

        printf("\n");
        printf("--- MAN PAGE ---\n");
        printf("\n");
        printf("Usage: preprocessor [flags] <program.c>\n");
        printf("\n");
        printf("Flags:\n");
        printf("  -c      Eliminate comments\n");
        printf("  -d      Replace all directives starting with #\n");
        printf("  -all    Do all processing (comments and directives)\n");
        printf("  -help   Print this man page\n");
        printf("\n");

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

    processFile(inputFile, outputFile);
}

