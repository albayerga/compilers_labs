#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------- */

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

/* --------------------------------------------- */

int removeComments(const char *input_file_name)
{
    char output_file_name[100];
    snprintf(output_file_name, sizeof(output_file_name), "%s_pp.c", input_file_name);

    FILE *input = fopen(input_file_name, "r");
    FILE *output = fopen(output_file_name, "w");

    if (input == NULL || output == NULL) {
        perror("Error opening files");
        return 1; // Return an error code
    }

    int c;
    int last = 0;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == '/' && last == '/')
        {
            while ((c = fgetc(input)) != EOF && c != '\n');
            last = 0;
        }
        else if (c == '*' && last == '/')
        {
            while ((c = fgetc(input)) != EOF && (c != '/' || last != '*'))
            {
                last = c;
            }
            last = 0;
        }
        else
        {
            fputc(last, output);
            last = c;
        }
    }

    fclose(input);
    fclose(output);

    return 0;
}

/* --------------------------------------------- */
void processDefine(const char *input_file_name)
{
    //...
}


/* --------------------------------------------- */

void handleInclude(FILE *output, const char *filename)
{
    FILE *includedFile = fopen(filename, "r");

    if (includedFile == NULL)
    {
        perror("Error opening included file");
    }

    int c;
    while ((c = fgetc(includedFile)) != EOF)
    {
        fputc(c, output);
    }

    fclose(includedFile);
}


void processInclude(const char *input_file_name)
{
    char output_file_name[100];
    snprintf(output_file_name, sizeof(output_file_name), "%s_pp.c", input_file_name);

    FILE *input = fopen(input_file_name, "r");
    FILE *output = fopen(output_file_name, "w");

    if (input == NULL || output == NULL)
    {
        perror("Error opening files");
    }

    int c;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == '#')
        {
            char directive[100];
            fscanf(input, "%99s", directive);
            if (strcmp(directive, "include") == 0)
            {
                char filename[100];
                fscanf(input, " \"%99[^\"]\"", filename); // Read the filename between quotes

                // Assume included files are in the same directory as the source file
                handleInclude(output, filename);
                continue;
            }
            else
            {
                fprintf(output, "#%s", directive);
            }
        }
        fputc(c, output);
    }
    fclose(input);
    fclose(output);
}

/* --------------------------------------------- */

//MAIN FUNCTION

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: preprocessor [flags] <program.c>\n");
        return 0;
    }

    //help
    if (argc == 2 && strcmp(argv[1], "-help") == 0)
    {
        printManPage();
        return 0;
    }

    //-c
    if (argc >= 2 && strcmp(argv[1], "-c") == 0)
    {
        if(argc != 3)
        {
            printf("Usage: preprocessor -c <program.c>\n");
            return 0;
        }
        else
        {
            const char *filename = argv[2];
            removeComments(filename);
        }
    }

    //-d
    if (argc >= 2 && strcmp(argv[1], "-d") == 0)
    {
        if(argc != 3)
        {
            printf("Usage: preprocessor -c <program.c>\n");
            return 0;
        }
        else
        {
            const char *filename = argv[2];
            processInclude(filename);
        }
    }
    
    //-all
    if (argc >= 2 && strcmp(argv[1], "-all") == 0)
    {
        printf("This will do all processing (comments and directives)\n");
    }

    return 0;
}