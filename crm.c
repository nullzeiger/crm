/*
  Copyright (C) 2024 Ivan Guerreschi <ivan.guerreschi.dev@gmail.com>.

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

#include "crm.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define MAX_PATH_LENGTH 4096
#define DEFAULT_FILE_PERMISSIONS 0644

/**
 * Copies a file to /tmp directory.
 *
 * @param filename The name of the file to copy
 * @return EXIT_SUCCESS on successful copy, otherwise exits with error
 */
int copy(const char *filename)
{
    /* Validate input parameters */
    if (filename == NULL) {
        error(EXIT_FAILURE, 0, "Filename is NULL");
    }

    /* Ensure filename isn't too long */
    size_t filename_len = strlen(filename);
    if (filename_len >= MAX_PATH_LENGTH - 6) {  /* 6 for "/tmp/" and null terminator */
        error(EXIT_FAILURE, 0, "Filename too long");
    }

    /* Prepare source path */
    char src_path[MAX_PATH_LENGTH];
    if (snprintf(src_path, MAX_PATH_LENGTH, "%s", filename) < 0) {
        error(EXIT_FAILURE, 0, "Error creating source path string");
    }

    /* Prepare destination path */
    char dest_path[MAX_PATH_LENGTH];
    if (snprintf(dest_path, MAX_PATH_LENGTH, "/tmp/%s", filename) < 0) {
        error(EXIT_FAILURE, 0, "Error creating destination path string");
    }

    /* Open source file for reading */
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        error(EXIT_FAILURE, errno, "Error opening source file '%s'", filename);
    }

    /* Open destination file for writing with appropriate permissions */
    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_FILE_PERMISSIONS);
    if (dest_fd == -1) {
        close(src_fd);
        error(EXIT_FAILURE, errno, "Error opening destination file '/tmp/%s'", filename);
    }

    /* Get file information for sendfile */
    struct stat stat_buf;
    if (fstat(src_fd, &stat_buf) == -1) {
        close(src_fd);
        close(dest_fd);
        error(EXIT_FAILURE, errno, "Error getting file status for '%s'", filename);
    }

    /* Copy file contents using sendfile */
    off_t offset = 0; /* Track current position in source file */
    size_t remaining = stat_buf.st_size;
    
    while (remaining > 0) {
        ssize_t bytes_sent = sendfile(dest_fd, src_fd, &offset, remaining);
        
        if (bytes_sent <= 0) {
            if (errno == EINTR) {
                /* Interrupted by signal, try again */
                continue;
            }
            close(src_fd);
            close(dest_fd);
            error(EXIT_FAILURE, errno, "Error copying file '%s'", filename);
        }
        
        remaining -= bytes_sent;
    }

    /* Close file descriptors */
    if (close(src_fd) == -1) {
        close(dest_fd); /* Try to close the other descriptor anyway */
        error(EXIT_FAILURE, errno, "Error closing source file '%s'", filename);
    }

    if (close(dest_fd) == -1) {
        error(EXIT_FAILURE, errno, "Error closing destination file '/tmp/%s'", filename);
    }

    return EXIT_SUCCESS;
}

/**
 * Deletes a file from the filesystem.
 *
 * @param filename The name of the file to delete
 * @return EXIT_SUCCESS on successful deletion, otherwise exits with error
 */
int delete(const char *filename)
{
    /* Validate input parameters */
    if (filename == NULL) {
        error(EXIT_FAILURE, 0, "Filename is NULL");
    }
    
    /* Check if file exists before attempting to delete it */
    if (access(filename, F_OK) == -1) {
        error(EXIT_FAILURE, errno, "File '%s' does not exist or is not accessible", filename);
    }

    /* Delete the file */
    if (unlink(filename) == -1) {
        error(EXIT_FAILURE, errno, "Error deleting file '%s'", filename);
    }

    return EXIT_SUCCESS;
}
