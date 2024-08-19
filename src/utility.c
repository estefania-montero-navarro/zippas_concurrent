// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>
#include "utility.h"
#include <errno.h>
uint64_t get_thread_amount(int argc, char* argv[]) {
  uint64_t thread_amount = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc > 1) {
    if (!(sscanf(argv[1], "%zu", &thread_amount)) ||
        (long)thread_amount > sysconf(_SC_NPROCESSORS_ONLN)) {
      thread_amount = sysconf(_SC_NPROCESSORS_ONLN);
    }
  }
  return thread_amount;
}

zip_password_t* read_input(password_information_t* pass_info) {
  pass_info->pass_counter = 0;
  zip_password_t* first_zip;
  char c[200];
  // pass_info->pass_alphabet := read_line() //check
  if (fgets(c, 200, stdin)) {
    uint64_t alphabet_size = strlen(c)-1;
    pass_info->pass_alphabet = (char*)calloc(alphabet_size, sizeof(char));
    strncpy(pass_info->pass_alphabet, c, alphabet_size);
    pass_info->alphabet_size = alphabet_size;  // no tiene \0
    // pass_info->max_pass_length := read_line() //check
    if (fgets(c, 200, stdin)) {
      pass_info->max_pass_length = atoi(c);
      if (pass_info->max_pass_length > 0) {
        // read_line()
        fgets(c, 200, stdin);
        fgets(c, 200, stdin);
        zip_password_t* last_pass = NULL;
        // while end_of_file not reached:
        while (!feof(stdin)) {
          // create zip_password := pass
          zip_password_t* zip_p = (zip_password_t*)calloc(1,
                sizeof(zip_password_t));
          if (pass_info->pass_counter == 0) {
              first_zip = zip_p;
          }
          //  pass->file_path := read_line()
          uint64_t path_len = strlen(c)-1;
          // pass->file_path := read_line()
          zip_p->file_path = (char*)calloc(path_len+1, sizeof(char));
          strncat(zip_p->file_path, c, path_len);
          // pass->password_info := pass_info
          zip_p->password_info = pass_info;
          // ++pass_info->pass_counter
          ++pass_info->pass_counter;
          if (last_pass != NULL) {
            last_pass->next_zip = (struct zip_password_t *)zip_p;
          }
          last_pass = zip_p;
          fgets(c, 200, stdin);
        }
      } else {
        first_zip = NULL;
        fprintf(stderr, "Invalid  maximum password size provided\n");
      }
    } else {
        first_zip = NULL;
        fprintf(stderr, "Invalid input, no maximum password size provided\n");
    }
  } else {
    first_zip = NULL;
    fprintf(stderr, "Invalid input, the file does not contain information\n");
  }
  return first_zip;
}

int print_results(zip_password_t* first_zip) {
  // current = first_zip
  zip_password_t* current = first_zip;
  // while current exists
  while (current) {
    // print(pass_array[i]->file_path+" "+pass_array[i]->password"\n")
    printf("%s ", current->file_path);
    if (current->password != NULL) {
      printf("%s\n", current->password);
    } else {
      printf("\n");
    }
    // current = next_zip
    current = (zip_password_t *)current->next_zip;
  }
  return EXIT_SUCCESS;
}

void free_zips(zip_password_t* first_zip) {
  if (first_zip->next_zip) {
    free_zips((zip_password_t *)first_zip->next_zip);
  }
  free(first_zip->file_path);
  free(first_zip->password);
  free(first_zip);
}

uint64_t get_file_size(FILE* file) {
  uint64_t file_size = 0;
  if (file) {
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
  }
  return file_size;
}

char* create_buffer(FILE* file, uint64_t file_size) {
  char* buffer = (char*)malloc(file_size);
  if (buffer) {
    fread(buffer, 1, file_size, file);
  }
  return buffer;
}

uint64_t len_elev_f(uint64_t alphabet_count, uint64_t size) {
  // num_elev = alphabet_count
  uint64_t num_elev = alphabet_count;
  // for i = 1 hasta size
  for (uint64_t i = 1; i < size; ++i) {
    // num_elev = num_elev * alphabet_count
    num_elev = num_elev * alphabet_count;
  }
  return num_elev;
}
