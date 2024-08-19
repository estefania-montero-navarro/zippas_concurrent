// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>
#include <stdio.h>
#include <stdlib.h>

#include "zippass_serial.h"

#if ZIPPASS_SERIAL
int find_password(zip_password_t* first_zip) {
  password_information_t* pass_info = first_zip->password_info;
  zip_password_t* current = first_zip;
  for (u_int64_t i = 0; i < pass_info->pass_counter; ++i) {
    current->zip_file = zip_open(current->file_path, 0, NULL);
    if (current->zip_file == NULL) {
      fprintf(stderr, "Could not open: %s\n", current->file_path);
      return FAILED_PROCEDURE;
    }
    current = (zip_password_t *)current->next_zip;
  }
  uint64_t passwords_found = 0;
  // file = open_zip()
  int error = EXIT_SUCCESS;
    // for size := 1 hasta max_pass_length
    for (uint64_t size = 1; size <= pass_info->max_pass_length; ++size) {
      // len_elev = len_elev(alphabet_count,size)
      uint64_t len_elev = len_elev_f(pass_info->alphabet_size, size);
      // for combination = 0 hasta len_elev
      for (uint64_t combination = 0; combination < len_elev; ++combination) {
        // num_array[size]
        uint64_t* num_array = (uint64_t*)calloc(size, sizeof(uint64_t));
        // transform_number_base(combination,size,alphabet_count,num_array)
        transform_number_base(combination, size, pass_info->alphabet_size,
             num_array);
        // password = transform_number_to_pass(alphabet,num_array,size)
        char* password = transform_number_to_pass(pass_info->pass_alphabet,
            num_array, size);
        free(num_array);
        zip_password_t* current = first_zip;
        for (uint64_t i = 0; i < pass_info->pass_counter; ++i) {
          if (current->password_found == 0) {
            uint64_t found = try_password(current->zip_file, password);
            if (found) {
              current->password_found = 1;
              char* pass_cpy = (char*)calloc(size+1, sizeof(char));
              strncpy(pass_cpy, password, size);
              current->password = pass_cpy;
              ++passwords_found;
            }
          }
          current = (zip_password_t *)current->next_zip;
        }
        free(password);
        if (passwords_found == pass_info->pass_counter) {
          combination = len_elev;
          size = pass_info->max_pass_length;
        }
      }
    }
  return error;
}


void transform_number_base(uint64_t number, uint64_t digit_amount,
    uint64_t new_base, uint64_t* num_array) {
  // --digit_amount
  --digit_amount;
  do {
    // num_array[digit_amount] = (number)mod(new_base)
    num_array[digit_amount] = number % new_base;
    // number = number/new_base
    number = number / new_base;
    // --digit_amount
    --digit_amount;
  // while number > 0
  } while (number != 0);
}

char* transform_number_to_pass(char* alphabet, uint64_t* num_array,
     uint64_t array_size) {
  char* password = (char*)calloc(array_size+1, sizeof(char));
  // for i=0 to array_size
  for (uint64_t i = 0; i < array_size; ++i) {
    // password = password + num_array[i]
    int pos = num_array[i];
    char ch = alphabet[pos];
    password[i] = ch;
  }
  password[array_size] = '\0';
  return password;
}

uint64_t try_password(zip_t* zip, char* password) {
  uint64_t found = 0;
  const char* const_pass = password;
  zip_file_t* is_pass = zip_fopen_index_encrypted(zip, 0,
      ZIP_FL_ENC_GUESS, const_pass);
  if (is_pass != NULL) {
    char res[11];
    char test[11] = "CI0117-23a";
    zip_int64_t can_read = zip_fread(is_pass, (void*)res, 10);
    if (can_read > 0) {
      uint64_t same = 1;
      for (int i = 0; i < 10; ++i) {
        if (res[i] != test[i]) {
          same = 0;
        }
      }
      if (same) {
        found = 1;
        zip_close(zip);
      }
    }
    zip_fclose(is_pass);
  }
  return found;
}

#endif
