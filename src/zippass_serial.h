// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>

// EXTERNAL HEATHERS
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <zip.h>



// OWN HEATHERS
#include "definitions.h"
#include "utility.h"

#if ZIPPASS_SERIAL


// FUNCTION DECLARATIONS

/**
 * @brief Find password function
 * 
 * Aims to find the password of the indicated zip file
 * 
 * @param pass_info Used to access the characteristics of the possible password
 * @param zip File whose password must be found
 * @return Code for the success or failure of the function
*/
int find_password(zip_password_t* first_zip);

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

/**
 * @brief Transform number base function
 * 
 * Transforms a number into a possible combination for the password
 * 
 * @param number Number to be transformed
 * @param digit_amount Amount of digits that the number array will have
 * @param new_base Base for the new number
 * @param num_array Array in which the number will be saved
*/
void transform_number_base(uint64_t number, uint64_t digit_amount,
    uint64_t new_base, uint64_t* num_array);

/**
 * @brief Transform number to password function
 * 
 * Transforms a number array into a viable password
 * 
 * @param alphabet Array of characters that the password may contain
 * @param num_array Array of numbers that refers to a possible combination
 * @param array_size Size of the password to be generated
 * @return Tha generated password
*/
char* transform_number_to_pass(char* alphabet, uint64_t* num_array,
     uint64_t array_size);

/**
 * @brief Checks if the given password corresponds to the file
 * 
 * @param zip File to be checked
 * @param password Password to be checked
 * @return 1 if the password is correct, 0 otherwise
*/
uint64_t try_password(zip_t* zip, char* password);
#endif
