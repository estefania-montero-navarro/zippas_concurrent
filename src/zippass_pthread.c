// Copyright 2023 Estefania Montero Navarro <estefania.monteronavarro@ucr.ac.cr>

#include "zippass_pthread.h"

#if ZIPPASS_PTHREAD

int find_password(zip_password_t *zip_info)  {
  queue_t *passwords_queue = (queue_t *)calloc(1, sizeof(queue_t));
  queue_init(passwords_queue);
  password_information_t *pass_info = zip_info->password_info;
  uint64_t check_thread_amount = pass_info->thread_amount/3;
  uint64_t pass_thread_amount = pass_info->thread_amount-check_thread_amount;
  pass_thread_data_t* pass_thread_data = create_pass_data(pass_thread_amount,
      passwords_queue, zip_info);
  check_thread_data_t* check_thread_data = create_check_data(check_thread_amount
      , passwords_queue, zip_info);
  run_threads(pass_info->thread_amount, pass_thread_data, check_thread_data);
  erase_temp_data(pass_thread_data, check_thread_data);
  return EXIT_SUCCESS;
}

pass_thread_data_t* create_pass_data(uint64_t data_amount,
    queue_t* passwords_queue, zip_password_t* zip_password )  {
  pass_thread_data_t* pass_thread_data = (pass_thread_data_t*)calloc(
      data_amount, sizeof(pass_thread_data_t));
  for (uint64_t i = 0; i < data_amount; ++i)  {
    pass_thread_data[i].passwords_queue = passwords_queue;
    pass_thread_data[i].pass_info = zip_password->password_info;
    pass_thread_data[i].zip_password = zip_password;
  }
  return pass_thread_data;
}

check_thread_data_t* create_check_data(uint64_t data_amount,
    queue_t* passwords_queue, zip_password_t* zip_password)  {
  // temp_data_arr = create temp_data_dynamic_array
  check_thread_data_t* temp_data_arr = (check_thread_data_t*)calloc(data_amount,
      sizeof(check_thread_data_t));
  // for (i=0...data_amount)
  if (temp_data_arr) {
    FILE* source_file = fopen(zip_password->file_path, "rb");
    if (!source_file) {
      fprintf(stderr, "Could not open %s\n", zip_password->file_path);
      // erase_temp_data(temp_data_arr, data_amount, 0);
      // temp_data_arr = NULL;
    } else {
      uint64_t file_size = get_file_size(fopen(zip_password->file_path, "rb"));
      char* buffer = create_buffer(source_file, file_size);
      for (uint64_t i = 0; i < data_amount; ++i) {
        // temp_file_path = "temp_dest/"+i
        char* text = "temp_dest/";
        size_t text_length = strlen(text);
        size_t path_length = text_length + 1 + snprintf(NULL, 0, "%d",
            (unsigned int)i);
        char* temp_file_path = (char*)malloc(path_length+1);
        snprintf(temp_file_path, path_length+1, "%s%d", text,
            (unsigned int)i);
        // current_file = create_temp_file(zip_password:file_path,temp_file_pat)
        if (!create_temp_file(temp_file_path, buffer, file_size)) {
          temp_data_arr[i].file_path = temp_file_path;
          temp_data_arr[i].zip_password = zip_password;
          temp_data_arr[i].passwords_queue = passwords_queue;
          temp_data_arr[i].zip_file = zip_open(temp_data_arr[i].file_path, 0,
              NULL);
        } else {
          temp_data_arr = NULL;
          fprintf(stderr, "Could not create %zu files, failed at file %zu",
              data_amount, i);
        }
      }
      free(buffer);
    }
  }
  return temp_data_arr;
}

int create_temp_file(char *new_file_path, char *buffer, uint64_t fileSize)  {
  int error = EXIT_SUCCESS;
  FILE *destFile;
  // create(dest_file)
  destFile = fopen(new_file_path, "wb");
  if (!destFile) {
    mkdir("temp_dest", 0777);
    destFile = fopen(new_file_path, "wb");
  }
  if (!destFile) {
    fprintf(stderr, "Failed to open the destination file.\n");
    error = EXIT_FAILURE;
  }
  // write_file(buf, dest_file)
  // printf("%s ratata\n", buffer);
  if (fwrite(buffer, 1, fileSize, destFile) != fileSize) {
    fprintf(stderr, "Failed to write to the file.\n");
    fclose(destFile);
    error = EXIT_FAILURE;
  }
  fclose(destFile);
  return error;
}

void run_threads(uint64_t team_size, pass_thread_data_t* pass_thread_data,
        check_thread_data_t* check_thread_data)  {
  // threads = thread_dynamic_array(team_size)
  uint64_t check_thread_amount = team_size/3;
  uint64_t pass_thread_amount = team_size-check_thread_amount;
  pthread_t *pass_threads = (pthread_t *)calloc(pass_thread_amount,
      sizeof(pthread_t));
  pthread_t *check_threads = (pthread_t *)calloc(check_thread_amount,
      sizeof(pthread_t));
  password_information_t *pass_info = check_thread_data[0].
      zip_password->password_info;
  if (pass_threads && check_threads) {
    for (uint64_t i = 0; i < check_thread_amount; ++i) {
      pthread_create(&check_threads[i], NULL, check_passwords,
          (void *)&check_thread_data[i]);
    }
    uint64_t max_size = pass_info->max_pass_length;
    for (uint64_t size = 1; size <= max_size; ++size) {
      // len_elev = len_elev(alphabet_count,size)
      // distribute_combinations(temp_data, len_elev)
        uint64_t len_elev = len_elev_f(pass_info->alphabet_size, size);
        distribute_combinations(pass_thread_data, len_elev, size,
            pass_thread_amount);
        for (uint64_t i = 0; i < pass_thread_amount; ++i) {
          //  threads[create_thread(routine, temp_data[i])]
          pthread_create(&pass_threads[i], NULL, look_for_password,
            (void *)&pass_thread_data[i]);
        }
        for (uint64_t i = 0; i < pass_thread_amount; ++i) {
          //  join_thread(threads[i])
          pthread_join(pass_threads[i], NULL);
        }
        if (get_passwords_found(check_thread_data[0].zip_password))  {
          size = max_size+1;
        }
    }
    for (uint64_t i = 0; i < check_thread_amount; ++i) {
      pthread_join(check_threads[i], NULL);
    }
    free(pass_threads);
    free(check_threads);
  } else {
    fprintf(stderr, "Could not create %zu threads\n", team_size);
  }
}

void distribute_combinations(pass_thread_data_t* temp_data,
        uint64_t total_combos, uint64_t size, uint64_t thread_amount) {
  uint64_t distribution = total_combos / thread_amount;
  uint64_t rest = total_combos % thread_amount;
  uint64_t comb_ini = 0;
  for (uint64_t i = 0; i < thread_amount; ++i) {
    temp_data[i].current_size = size;
    temp_data[i].comb_ini = comb_ini;
    temp_data[i].comb_end = comb_ini + distribution;
    if (i < rest) {
      ++temp_data[i].comb_end;
    }
    comb_ini = temp_data[i].comb_end;
  }
}

void* look_for_password(void *data) {
  pass_thread_data_t *temp_data = (pass_thread_data_t *)data;
  password_information_t *pass_info = temp_data->pass_info;
  // file = open_zip()
  uint64_t len_elev = temp_data->comb_end;
  uint64_t size = temp_data->current_size;
  // num_array[size]
  uint64_t *num_array = (uint64_t *)calloc(size, sizeof(uint64_t));
  // for combination = 0 hasta len_elev
  for (uint64_t combination = temp_data->comb_ini; combination < len_elev;
       ++combination) {
    if (get_passwords_found(temp_data->zip_password)) {
      combination = len_elev;
      printf("stopped\n");
    } else {
      // transform_number_base(combination,size,alphabet_count,num_array)
      transform_number_base(combination, size, pass_info->alphabet_size,
                            num_array);
      // password = transform_number_to_pass(alphabet,num_array,size)
      char *password = transform_number_to_pass(pass_info->pass_alphabet,
                                                num_array, size);
      char *const_pass = password;
      queue_enqueue(temp_data->passwords_queue, const_pass);
    }
  }
  // queue_enqueue(temp_data->passwords_queue, NULL);
  free(num_array);
  return NULL;
}

void *check_passwords(void *data) {
  check_thread_data_t* temp_data = (check_thread_data_t *)data;
  zip_password_t *current = temp_data->zip_password;
  uint64_t found = 0;
  while (!found) {
    char *password = NULL;
    queue_dequeue(temp_data->passwords_queue, &password);
    // printf("Dequeuin: %s\n", password);
      const char* const_pass = password;
      // if (try_password(file,num_array))
      zip_file_t* is_pass = zip_fopen_index_encrypted(temp_data->zip_file, 0,
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
              // combination = len_elev
              current->password = password;
                pthread_mutex_lock(&current->can_access_password_found);
                  current->password_found = 1;
                pthread_mutex_unlock(&current->can_access_password_found);
                found = 1;
            } else {
              free(password);
            }
        }
        zip_fclose(is_pass);
      } else {
        free(password);
      }
  }
  zip_close(temp_data->zip_file);
  free(temp_data->file_path);
  return NULL;
}

uint64_t try_password(zip_t *zip, char *password) {
  uint64_t found = 0;
  const char *const_pass = password;
  zip_file_t *is_pass = zip_fopen_index_encrypted(zip, 0,
      ZIP_FL_ENC_GUESS, const_pass);
  if (is_pass != NULL) {
    char res[11];
    char test[11] = "CI0117-23a";
    zip_int64_t can_read = zip_fread(is_pass, (void *)res, 10);
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

uint64_t get_passwords_found(zip_password_t *zip_info) {
  pthread_mutex_lock(&zip_info->can_access_password_found);
  uint64_t found = zip_info->password_found;
  pthread_mutex_unlock(&zip_info->can_access_password_found);
  return found;
}

void transform_number_base(uint64_t number, uint64_t digit_amount,
                           uint64_t new_base, uint64_t *num_array) {
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

char *transform_number_to_pass(char *alphabet, uint64_t *num_array,
                               uint64_t array_size) {
  char *password = (char *)calloc(array_size + 1, sizeof(char));
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

void erase_temp_data(pass_thread_data_t* pass_thread_data,
        check_thread_data_t* check_thread_data)  {
  queue_clear(pass_thread_data->passwords_queue);
  queue_destroy(pass_thread_data->passwords_queue);
  free(pass_thread_data->passwords_queue);
  free(pass_thread_data);
  free(check_thread_data);
}

#endif
