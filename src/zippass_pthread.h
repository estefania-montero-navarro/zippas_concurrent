// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>

// EXTERNAL LIBRARIES
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <zip.h>

// OWN HEATHERS
#include "definitions.h"
#include "utility.h"

#if ZIPPASS_PTHREAD

// FUNCTION DECLARATIONS

/**
 * @brief Find password function
 *
 * Aims to find the password of the indicated zip file
 *
 * @param zip File whose password must be found
 * @return Code for the success or failure of the function
 */
int find_password(zip_password_t *zip);

/**
 * @brief Creates temporary data to be passed to the password generator threads
 *
 * @param data_amount Amount of data sets to be created
 * @param passwords_queue Queue for the passwords generated
 * @param zip_password Necessary information to be added to each data set
 */
pass_thread_data_t *create_pass_data(uint64_t data_amount,
        queue_t *passwords_queue, zip_password_t *zip_password);

/**
 * @brief Creates temporary data to be passed to the password checker threads
 *
 * @param data_amount Amount of data sets to be created
 * @param passwords_queue Queue for the passwords generated
 * @param first_zip Necessary information to be added to each data set
 */
check_thread_data_t *create_check_data(uint64_t data_amount,
        queue_t *passwords_queue, zip_password_t *zip_pass);

/**
 * @brief Generates copies of the zip files
 *
 * @param first_zip First zip file
 * @param data Data for the password checker threads
 * @param data_amount Amount of copies to be generated
 */
void generate_file_copies(zip_password_t *first_zip,
                          check_thread_data_t *data, uint64_t data_amount);

/**
 * @brief Creates the temporary data to be passed to the threads
 *
 * @param data_amount The amount of data sets that need to be created
 * @param pass_thread_data Data for the passwords finder threads
 * @param zip_password Necessary information to be added to each data set
 */
void create_temp_data(uint64_t data_amount,
        pass_thread_data_t *pass_thread_data, zip_password_t *zip_password);

/**
 * @brief Creates a temporary file by copying an existing one
 *
 * @param file_path New file's path
 * @param source_file File to be copied
 * @return exit or success code
 *
 */
int create_temp_file(char *new_file_path, char *buffer,
                     uint64_t size);

/**
 * @brief Starts the threads to find the password and joins them afterwards
 *
 * @param team_size Total amount of threads
 * @param pass_thread_data Data for the passwords finder threads
 * @param check_thread_data Data for the password checker threads
 */
void run_threads(uint64_t team_size, pass_thread_data_t *pass_thread_data,
                 check_thread_data_t *check_thread_data);

/**
 * @brief Distributes the combinations to be generated by each thread
 *
 * @param temp_data Temporary data sets to be passed to each thread
 * @param total_combos Total amount of combinations to be generated
 * @param size Size of the password
 * @param thread_amount Amount of threads to be used
 */
void distribute_combinations(pass_thread_data_t *temp_data,
        uint64_t total_combos, uint64_t size, uint64_t thread_amount);

/**
 * @brief Looks for the password at a certain size
 *
 * @param temp_data Temporary data sets to be passed to each thread
 */
void *look_for_password(void *data);

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
                           uint64_t new_base, uint64_t *num_array);

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
char *transform_number_to_pass(char *alphabet, uint64_t *num_array,
                               uint64_t array_size);

/**
 * @brief Checks if the password is correct
 *
 * @param data Temporary data sets to be passed to each thread
 */
void *check_passwords(void *data);

/**
 * @brief Erases the temporary data generated
 *
 * @param pass_thread_data Password finder threads data
 * @param check_thread_data Password checker threads data
 */
void erase_temp_data(pass_thread_data_t *pass_thread_data,
                     check_thread_data_t *check_thread_data);

/**
 * @brief Tries a password in a zip file
 *
 * @param zip Pointer to the zip file
 * @param password Password to be tried
 * @return 1 if the password is correct, 0 otherwise
 */
uint64_t try_password(zip_t *zip, char *password);

/**
 * @brief Checks if the password has been found
 *
 * @param zip_info Pointer to the zip's information
 * @return The amount of passwords found
 */
uint64_t get_passwords_found(zip_password_t *zip_info);

/**
 * @brief Increments the amount of passwords found
 *
 * @param zip_info Pointer to the zips' information
 */
void increment_passwords_found(password_information_t *zip_info);
#endif
