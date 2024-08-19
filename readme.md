# Zip Pass Serial

## Project description

This project aims to optimize the previous solutions to the zip password problem. For both the serial and concurrent approach there must be a noticeable improvement in the execution time.

As with the previous programs, it requires the passwords 'alphabet', which refers to the possible characters it may include, and the password's length, as well as the files which require decryption. This must be provided through standard input (see specifics below).

The program will check the different possible combinations until it finds (or not) the password. After this has been done for all the files indicated, the passwords will be returned through standard output (see specifics below).

Also, in case the user wishes for the program to generate a specific amount of threads, they must indicate this number as a program argument.

To check the optimization results go to [Report](/report/readme.md)

To check the project's design diagrams go to [Design readme](/design/readme.md)

## User manual

The necessary information for the program's proper functioning must be provided must follow the following format:

Password alphabet
Password length
Empty line
Path to the first file (relative)
.
.
.
Path to the last file

Input example:
0123456789
5

tests/zip_05/f01.zip
tests/zip_05/f23.zip
tests/zip_05/f09.zip

After the passwords have been found (or not), they will be displayed next to the file path after a 'space'. If the password was not found there will be nothing next to the path.

Output example:
tests/zip_05/f01.zip 00112
tests/zip_05/f23.zip
tests/zip_05/f09.zip 9209

**Run instructions**
To run the program using a .txt file, add the input as shown before and use a command with the following structure:
"executable_file < input.txt"

In case the user wishes to use a specific amount of threads:
"executable_file 'thread_amount' < input.txt"

## Compilation instructions

To choose between the serial and concurrent implementation, the constants 'ZIPPASS_SERIAL' and 'ZIPPASS_PTHREAD' must be defined to 1 or 0, depending on which one is wished for, these constants are defined in [definitions](src/definitions.h).

To compile, simply run the command make while on the project directory.

## Credits

Project designed and implemented by Estefanía Montero Navarro <estefania.monteronavarro@uc.ac.cr>
