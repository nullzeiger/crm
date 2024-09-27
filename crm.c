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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <error.h>

int
main (int argc, char *argv[])
{
  /* Exactly two arguments. */
  if (argc != 2)
    {
      error (EXIT_FAILURE, 0, "Usage: %s <filename>", "crm");
    }

  const char *filename = argv[1];

  /*  Checks the file access permissions for the specified file filename.
      The F_OK flag indicates that only the existence of the file
      is being checked, without verifying any permissions. */
  if (access (filename, F_OK) == -1)
    {
      error (EXIT_FAILURE, errno, "File %s does not exist", filename);
    }

  char *command;
  FILE *fp=NULL;
  const char *cp = "cp %s %s";
  const char *tmp = "/tmp";

  /* Allocate memory for the command variable */
  command = malloc (strlen (cp) + strlen (filename) + strlen (tmp));
  if (command == NULL)
    {
      error (EXIT_FAILURE, errno, "Memory allocation failed");
    }

  /* Composes a string command */
  sprintf (command, cp, filename, tmp);

  /* Open a pipe for reading the output of a command. */
  if ((fp = popen (command, "r")) == NULL)
    {
      free (command);
      error (EXIT_FAILURE, errno, "Error move %s in /tmp", filename);
    }

  /* Deallocate memory for the command variable */
  free (command);

  /* Close the pipe associated with the file pointer fp. */
  pclose (fp);

  /* Delete a specified file. */
  if (unlink (filename) == -1)
    {
      error (EXIT_FAILURE, errno, "Error deleting file %s", filename);
    }

  printf ("File %s has been successfully deleted.\n", filename);

  return EXIT_SUCCESS;
}
