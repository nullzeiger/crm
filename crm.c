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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

int
copy (const char *filename)
{
  /* Set path source and destination. */
  char src_path[256], dest_path[256];
  snprintf (src_path, sizeof (src_path), "%s", filename);	/* root */
  snprintf (dest_path, sizeof (dest_path), "/tmp/%s", filename);

  /* Open the source file for reading. */
  int src_fd = open (src_path, O_RDONLY);
  if (src_fd == -1)
    {
      error (EXIT_FAILURE, errno, "Error opening source file %s", filename);
    }

  /* Open the destination file for writing. */
  int dest_fd = open (dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dest_fd == -1)
    {
      error (EXIT_FAILURE, errno, "Error opening destination file %s",
	     filename);
    }

  /* Get the size of the source file. */
  struct stat stat_buf;
  fstat (src_fd, &stat_buf);

  /* Copy the entire file in one system call. */
  ssize_t bytes_sent = sendfile (dest_fd, src_fd, NULL, stat_buf.st_size);
  if (bytes_sent == -1)
    {
      close (src_fd);
      close (dest_fd);
      error (EXIT_FAILURE, errno, "Error copying file %s", filename);
    }

  close (src_fd);
  close (dest_fd);

  return EXIT_SUCCESS;
}

int
delete (const char *filename)
{
  /* Call the unlink function to remove the specified file. */
  if (unlink (filename) == -1)
    {
      error (EXIT_FAILURE, errno, "Error deleting file %s", filename);
    }

  return EXIT_SUCCESS;
}
