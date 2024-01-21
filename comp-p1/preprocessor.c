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
        printf("This will eliminate comments\n");
        return 0;
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