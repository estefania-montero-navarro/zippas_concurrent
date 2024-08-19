// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>

#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create password_information
  password_information_t* password_info = (password_information_t*)
      calloc(1, sizeof(password_information_t));
  #if ZIPPASS_PTHREAD
  // get_thread_amount(argc,argv)
  password_info->thread_amount = get_thread_amount(argc, argv);
  #endif
  // zip_password first_zip
  zip_password_t* first_zip = NULL;
  // read_input(password_information,first_zip)
  first_zip = read_input(password_info);

  if (first_zip != NULL) {
    #if ZIPPASS_PTHREAD
    // current := first_zip
    zip_password_t* current = first_zip;
    // while current exists
    while (current) {
      // find_password(password_information,current)
      error = find_password(current);
      printf("Password for %s is %s\n", current->file_path,
          current->password);
      if (error) {
        current->password = "Not a valid path";
      }
      // current := next_after_current
      current = (zip_password_t *)current->next_zip;
    }
    #elif ZIPPASS_SERIAL

      error = find_password(first_zip);
    #endif
    // print_results(zip_array,password_information->pass_counter)
    error = print_results(first_zip);
  }
  // delete zip_array
  if (first_zip) {
    free_zips(first_zip);
  }
  // delete password_info
  free(password_info->pass_alphabet);
  free(password_info);
  return error;
}
