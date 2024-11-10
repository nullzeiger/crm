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
int copy(const char *filename)
{
	/* Check if the provided filename is NULL. */
	if (filename == NULL) {
		error(EXIT_FAILURE, 0, "Filename is NULL");
	}

	/* Calculate the length needed to store the source file path. */
	size_t src_len = strlen(filename) + 1;
	/* Dynamically allocate memory to store the source path. */
	char *src_path = malloc(src_len);
	if (!src_path) {
		error(EXIT_FAILURE, errno, "Memory allocation error");
	}

	/* Copy the filename to the source path buffer. */
	snprintf(src_path, src_len, "%s", filename);

	/* Open the source file for reading. */
	int src_fd = open(src_path, O_RDONLY);
	if (src_fd == -1) {
		free(src_path);
		src_path = NULL;
		error(EXIT_FAILURE, errno, "Error opening source file %s",
		      filename);
	}

	/* Calculate the length needed for the destination path in the /tmp directory. */
	size_t dest_len = strlen("/tmp/") + strlen(filename) + 1;
	/* Dynamically allocate memory for the destination path. */
	char *dest_path = malloc(dest_len);
	if (!dest_path) {
		free(src_path);
		src_path = NULL;
		close(src_fd);
		src_fd = -1;
		error(EXIT_FAILURE, errno, "Memory allocation error");
	}

	/* Construct the destination path in the /tmp directory. */
	snprintf(dest_path, dest_len, "/tmp/%s", filename);

	/* Open the destination file for writing. */
	int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dest_fd == -1) {
		free(src_path);
		src_path = NULL;
		close(src_fd);
		src_fd = -1;
		error(EXIT_FAILURE, errno,
		      "Error opening destination file %s", filename);
	}

	/* Get the size of the source file. */
	struct stat stat_buf;
	if (fstat(src_fd, &stat_buf) == -1) {
		free(src_path);
		src_path = NULL;
		free(dest_path);
		dest_path = NULL;
		close(src_fd);
		src_fd = -1;
		close(dest_fd);
		dest_fd = -1;
		error(EXIT_FAILURE, errno,
		      "Error getting file status for %s", filename);
	}

	/* Copy the entire file content from source to destination using sendfile(). */
	ssize_t bytes_sent =
	    sendfile(dest_fd, src_fd, NULL, stat_buf.st_size);
	if (bytes_sent == -1) {
		free(src_path);
		src_path = NULL;
		free(dest_path);
		dest_path = NULL;
		close(src_fd);
		src_fd = -1;
		close(dest_fd);
		dest_fd = -1;
		error(EXIT_FAILURE, errno, "Error copying file %s",
		      filename);
	}


	/* Close the file descriptors */
	if (close(src_fd) == -1) {
		error(EXIT_FAILURE, errno, "Error closing source file %s",
		      filename);
	}

	if (close(dest_fd) == -1) {
		error(EXIT_FAILURE, errno,
		      "Error closing destination file %s", filename);
	}

	/* Free dynamically allocated memory for source and destination paths. */
	free(src_path);
	free(dest_path);

	return EXIT_SUCCESS;
}

int delete(const char *filename)
{
	/* Call the unlink function to remove the specified file. */
	if (unlink(filename) == -1) {
		error(EXIT_FAILURE, errno, "Error deleting file %s",
		      filename);
	}

	return EXIT_SUCCESS;
}
