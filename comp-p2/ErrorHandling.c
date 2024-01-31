#include "ErrorHandling.h"
#include <stdio.h>
#include <stdarg.h>

void handleError(enum ErrorIdentifier errorId, enum StepIdentifier step, char* text)
{
    if (DEBUG)
    {
        switch (errorId)
        {
            case ERROR_INVALID_USAGE:
                fprintf(stderr, "Error in %s: %s\n", step == STEP_SCANNER ? "Scanner" : "Parser", text);
                break;
            case ERROR_FILE_NOT_FOUND:
                fprintf(stderr, "Error in %s: File %s not found\n", step == STEP_SCANNER ? "Scanner" : "Parser", text);
                break;
            // add more as needed
            default:
                fprintf(stderr, "Unknown error\n");
        }
    }
}