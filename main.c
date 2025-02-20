/*
  Copyright (C) 2025 Ivan Guerreschi <ivan.guerreschi.dev@gmail.com>.
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file main.c
 * @brief CRM (Copy and Remove Manager) main program
 *
 * This program copies a file to /tmp directory and then deletes the original,
 * effectively providing a "move to temporary storage" functionality.
 */

#include "crm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>

/* Program name for usage messages */
#define PROGRAM_NAME "crm"
#define VERSION "1.0.0"

/* Function prototypes for helper functions */
static void print_usage(const char *prog_name);
static void print_version(void);
static bool confirm_deletion(const char *filename);
static int perform_crm_operation(const char *filename, bool interactive);

/**
 * Main function - entry point for the CRM program
 */
int main(int argc, char *argv[])
{
    int opt;
    bool interactive = false;
    char *filename = NULL;

    /* Define long options for command line parsing */
    static struct option long_options[] = {
        {"interactive", no_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    /* Parse command line options */
    while ((opt = getopt_long(argc, argv, "ihv", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                interactive = true;
                break;
            case 'h':
                print_usage(PROGRAM_NAME);
                return EXIT_SUCCESS;
            case 'v':
                print_version();
                return EXIT_SUCCESS;
            default:
                print_usage(PROGRAM_NAME);
                return EXIT_FAILURE;
        }
    }

    /* Check if a filename was provided as a non-option argument */
    if (optind >= argc) {
        error(EXIT_FAILURE, 0, "Error: No filename specified");
    } else if (optind < argc - 1) {
        /* Too many arguments provided */
        error(EXIT_FAILURE, 0, "Error: Too many arguments. This program accepts only one filename");
    }

    /* Get the filename from command line arguments */
    filename = argv[optind];

    /* Perform the actual CRM operation */
    return perform_crm_operation(filename, interactive);
}

/**
 * Performs the copy and remove operation
 * 
 * @param filename The file to process
 * @param verbose Whether to display detailed messages
 * @param interactive Whether to prompt for confirmation before deletion
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
static int perform_crm_operation(const char *filename, bool interactive)
{
    /* Validate input */
    if (filename == NULL || *filename == '\0') {
        error(EXIT_FAILURE, 0, "Error: Invalid filename");
    }

    /* Check if file exists */
    if (access(filename, F_OK) == -1) {
        error(EXIT_FAILURE, errno, "Error: File '%s' does not exist or is not accessible", filename);
    }

    /* Check if file is readable */
    if (access(filename, R_OK) == -1) {
        error(EXIT_FAILURE, errno, "Error: File '%s' is not readable", filename);
    }

    /* Check if file is writable (needed for deletion) */
    if (access(filename, W_OK) == -1) {
        error(EXIT_FAILURE, errno, "Error: File '%s' is not writable and cannot be deleted", filename);
    }

    /* Attempt to copy the file */
    if (copy(filename) != EXIT_SUCCESS) {
        /* Copy function handles its own error reporting */
        return EXIT_FAILURE;
    }

    /* If interactive mode is enabled, ask for confirmation before deletion */
    if (interactive) {
        if (!confirm_deletion(filename)) {
            printf("Operation cancelled. File '%s' preserved.\n", filename);
            return EXIT_SUCCESS;
        }
    }

    /* Attempt to delete the original file */
    if (delete(filename) != EXIT_SUCCESS) {
        /* Delete function handles its own error reporting */
        return EXIT_FAILURE;
    }

    /* Report successful completion */
    printf("File '%s' has been successfully moved to /tmp\n", filename);
    return EXIT_SUCCESS;
}

/**
 * Requests user confirmation before deleting a file
 * 
 * @param filename The file to be deleted
 * @return true if user confirms, false otherwise
 */
static bool confirm_deletion(const char *filename)
{
    char response[10];
    
    printf("Do you want to delete '%s'? [y/N]: ", filename);
    
    /* Get user input, with safeguards */
    if (fgets(response, sizeof(response), stdin) == NULL) {
        return false;  /* Default to safety on input error */
    }
    
    /* Check if the first character is 'y' or 'Y' */
    return (response[0] == 'y' || response[0] == 'Y');
}

/**
 * Displays program usage information
 * 
 * @param prog_name The name of the program
 */
static void print_usage(const char *prog_name)
{
    printf("Usage: %s [OPTIONS] FILENAME\n\n", prog_name);
    printf("Copy a file to /tmp directory and remove the original.\n\n");
    printf("Options:\n");
    printf("  -i, --interactive   Prompt for confirmation before deleting\n");
    printf("  -h, --help          Display this help and exit\n");
    printf("  -v, --version       Output version information and exit\n\n");
    printf("Examples:\n");
    printf("  %s document.txt     Move document.txt to /tmp\n", prog_name);
    printf("  %s -i important.doc Move with confirmation output\n\n", prog_name);
    printf("Report bugs to: ivan.guerreschi.dev@gmail.com\n");
}

/**
 * Displays version information
 */
static void print_version(void)
{
    printf("%s %s\n", PROGRAM_NAME, VERSION);
    printf("Copyright (C) 2025 Ivan Guerreschi <ivan.guerreschi.dev@gmail.com>\n");
    printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
