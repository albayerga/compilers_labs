To compile this project:

    make -f mymakefile

To execute this project with test1.c or test2.c:

    ./scanner test1.c
    ./scanner test2.c

Default execution mode for this project:

    DEBUG 1
    - This means debug mode is activated.
        Output will be an outputfile.
        When there's a sintax error, takes it as a token and it will be showed in the output. (you can see this with test2.c)
    - If you want to desactivate debug mode, change DEBUG to 0 in scanner.h.
        Output will be stdout.
        When there's a sintax error, skips it and continue reading the other tokens (we will be only notified in the console)

    OUTPUTFORMAT 1
    - This means debug outputformat is activated.
        Tokens will be like: (line number <lexeme, category> <lexeme, category> ... \n)
    - If you want to activate release mode, change OUTPUTFORMAT to 0 in scanner.h
        tokens will be like: (<lexeme, category> <lexeme, category> ...).

    COUNTOP 1
    - This means operations will be counted for every token and we will have the sum at the end of the file.
        (OUTPUTFORMAT must be 1 for this to happen).
    - If you want to desactivate the operation counting, change COUNTOP to 0.

If you make any changes to the execution mode, just recompile it with "make -f mymakefile".

Note: We have considered spaces and tabs as CAT_NONESSENTIAL because it is a category described in the practice paper.
Although we haven't considered "\n" as a token because every time we change line in the code, we will change line in the output.