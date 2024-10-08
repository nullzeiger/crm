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

#ifndef CRM_H
#define CRM_H

/*
 * Copies a file from the specified source to the `/tmp` directory.
 *
 * @param filename The name of the source file.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int copy(const char *filename);

/*
 * Deletes the specified file.
 *
 * @param filename The name of the file to delete.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int delete(const char *filename);

#endif
