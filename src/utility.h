// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>

#ifndef UTILITY_H
#define UTILITY_H
// EXTERNAL LIBRARIES
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// OWN HEATHERS
#include "definitions.h"
#include "data_structures.h"
#if ZIPPASS_SERIAL
#include "zippass_serial.h"
#elif ZIPPASS_PTHREAD
#include "zippass_pthread.h"
#endif
/**
 * @brief Get thread amount
 * 
 * Obtains the desired amount of threads to be used, if input, if not, uses the
 * maximum amount of threads used by the system
 * 
 * @param argc Amount of arguments input
 * @param argv Arguments input per se
 * @return The thread amount 
*/
uint64_t get_thread_amount(int argc, char* argv[]);

/**
 * @brief Read Input Function
 * 
 * Reads the input provided through standard input and assigns the information
 * to the corresponding variables
 * 
 * @param pass_info To be filled with the passwords' general information
 * @return zip_password_t pointer to the first zip_password instance
 */
zip_password_t* read_input(password_information_t* pass_info);

/**
 * @brief Print Results Function
 * 
 * Prints the password of each zip file provided, if it was found
 * 
 * @param first_zip Access to first zip to print
 * @return Code for the success or failure of the function
*/
int print_results(zip_password_t* first_zip);

/**
 * @brief Free zips function
 * 
 * Frees the memory of each zip_password_t instance
 * 
 * @param first_zip Access to the first zip
*/
void free_zips(zip_password_t* first_zip);

/**
 * @brief Gets the size of a file
 * 
 * @param file_path Path to the file
 * @return The size of the file
*/
uint64_t get_file_size(FILE* file);

/**
 * @brief Creates a buffer to store the file's content
 * 
 * @param file File to be read
 * @param size Size of the file
 * @return The buffer
*/
char* create_buffer(FILE* file, uint64_t file_size);

/**
 * @brief Calculate a power of len function
 * 
 * Calculates the amount of combinations possible for a certain password size
 * 
 * @param alphabet_count The amount of characters in the password's alphabet
 * @param size Size of this password
 * @return The result of the power
*/
uint64_t len_elev_f(uint64_t alphabet_count, uint64_t size);
#endif
