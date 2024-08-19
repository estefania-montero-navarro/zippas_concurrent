// Copyright 2021 Estefanía Montero <estefania.monteronavarro@ucr.ac.cr>
// CC-BY 4.0
/**
 * This file contains the definition of different error codes and other macros
*/

#define NO_ARG_PROVIDED  101  // A certain argument wasn't provided upon runtime
#define INVALID_ARG 102
#define NO_PARAM_PROVIDED 103  // A certain parameter wasn't provided
#define INVALID_PARAM 104  // The parameter provided is invalid
#define FAILED_PROCEDURE_M 105  // A self-made procedure failed
#define FAILED_PROCEDURE  106  // A non self-made procedure failed
#define ALLOC_ERROR 107  // Couldn't allocate the desired memory
#define CREATE_THREAD_ERROR 108  // Couldn't create thread
#define CANT_OPEN_FILE 201
#define INVALID_INPUT 202

// Macros
#define ZIPPASS_PTHREAD 1
#define ZIPPASS_SERIAL 0
