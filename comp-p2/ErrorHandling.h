#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H
#define DEBUG 1

// Error identifiers
enum ErrorIdentifier
{
    ERROR_INVALID_USAGE = 1,
    ERROR_FILE_NOT_FOUND = 2,
    // add more as needed
};

// Step identifiers
enum StepIdentifier
{
    STEP_SCANNER = 1,
    STEP_PARSER = 2,
};

// Function declarations
void handleError(enum ErrorIdentifier errorId, enum StepIdentifier step, char* text);

#endif  // ERROR_HANDLING_H

