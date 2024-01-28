#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

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

//TAKE WORD
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

char *takeWord(FILE *inputFile, long *filePosition)
{
    fseek(inputFile, *filePosition, SEEK_SET);
    char *word = malloc(100);
    int i = 0;
    char c;
    while ((c = fgetc(inputFile)) != EOF)
    {
        if(c == ' ' || c == '\n' || c == '\t' || specialWord(word) == TRUE)
        {
            word[i] = '\0';
            *filePosition = ftell(inputFile);
            return word;
        }
        else
        {
            word[i] = c;
            i++;
        }
    }
    word[i] = '\0';
    *filePosition = ftell(inputFile);
    return word;
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
    //read the file, when you find a key word, if the flag is on, process it

    long filePosition = 0;
    while(!feof(inputFile))
    {
        copy = TRUE;

        char* word = takeWord(inputFile, &filePosition);
        //filePosition = ftell(inputFile); //save position to read next word

        for(int i = 0; i < dictionarySize; i++)
        {
            if(strcmp(dictionary[i].key, word) == 0) //si word está en el diccionario, sustituirlo por su valor
            {
                word = dictionary[i].value;
                break;
            }
        }
        
        //comprobar keywords y flags activadas
        if((strcmp(word,"#define") == 0) & (flags.processDirectives == TRUE))
        {
            //call processDefine
            copy = FALSE;
        }

        if((strcmp(word,"#include") == 0) & (flags.processInclude == TRUE)) //al final será processDirectives
        { 
            word = takeWord(inputFile, &filePosition);
            if(word != NULL)
            {
                processInclude(inputFile, outputFile, word);
            }
            //call processInclude
                //processIncludedProgramFiles (like "comp-p1.h")
                //processIncludedLibraryFiles (like <stdio.h>)
            copy = FALSE;
        }

        if((strcmp(word,"#ifdef") == 0) & (flags.processIfDef == TRUE)) //al final será processDirectives
        {
            //call processIfDef
            copy = FALSE;
        }

        if(((strcmp(word,"//") == 0) || (strcmp(word,"/*"))) && (flags.processComments == TRUE))
        {
            //call processComments
            copy = FALSE;
        }


        if(copy == TRUE)
        {
            fprintf(outputFile, "%s", word);
        }

        free(word);
    }
    fclose(inputFile);
    fclose(outputFile);
}