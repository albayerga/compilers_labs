#include <stdio.h>
#include <string.h>

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

    // Code to read input file, remove comments, and write to output file
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
        printf("This will replace all directives starting with #\n");
        return 0;
    }
    
    //-all
    if (argc >= 2 && strcmp(argv[1], "-all") == 0)
    {
        printf("This will do all processing (comments and directives)\n");
        return 0;
    }

    return 0;
}