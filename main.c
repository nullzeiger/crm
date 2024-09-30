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

  /* Copy and remove file. */
  copy (filename);
  delete (filename);

  printf ("File %s has been successfully deleted.\n", filename);

  return EXIT_SUCCESS;
}
