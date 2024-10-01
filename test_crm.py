#!/usr/bin/env python3

# Copyright (C) 2024 Ivan Guerreschi <ivan.guerreschi.dev@gmail.com>.
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


import os
import subprocess
import unittest


# Define a test class for `./crm` program
class TestCRM(unittest.TestCase):
    
    # Test case for removing a file
    def test_file_removal(self):
        args = ['./crm', 'test.txt']

        # Create file test.txt
        file = open(args[1], 'w')
        lorem = ''' Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras rutrum sollicitudin enim, sit amet accumsan nisl mollis at. Pellentesque lobortis, tortor id mattis laoreet, ex justo mollis sem, nec varius nisl ligula vel augue. Pellentesque in enim viverra, tempor purus ac, porta sem. Phasellus egestas ut nibh non venenatis. Vivamus a orci tincidunt, pharetra velit viverra, tempus mauris. Sed mollis erat neque, sit amet condimentum sem pretium suscipit. Aliquam erat volutpat. Suspendisse nec leo dignissim, egestas ligula sed, tristique tortor. Maecenas non lorem tempus, molestie augue eget, luctus enim. Praesent malesuada augue et dapibus imperdiet. Nulla facilisi. Sed urna diam, vehicula nec tortor ac, lacinia maximus odio. In lobortis tortor ligula. Praesent risus libero, pharetra vel pretium eu, tristique congue nibh. Quisque tincidunt a diam ac tincidunt. Nullam a ultricies neque. '''
        file.write(lorem)
        file.close()

        # Call the `./crm` program with the defined arguments
        subprocess.run(args)

        # Check if the file 'test.txt' still exists in the `/tmp` directory
        self.assertTrue(os.path.exists(f"/tmp/{args[1]}"))
        
        # Check if the file 'test.txt' exists in the current directory (root directory)
        self.assertFalse(os.path.exists(args[1]))

    # Test case for handling multiple arguments
    def test_more_arguments(self):
        # Define arguments with additional words
        args = ['./crm', 'hello', 'world']

        # Call the `./crm` program with these arguments
        test_more_arguments = subprocess.run(args)

        # Check if the program returned an error code (non-zero)
        self.assertTrue(test_more_arguments.returncode)

    # Test case for handling zero arguments
    def test_zero_argument(self):
        # Define arguments with no additional arguments
        args = './crm'

        # Call the `./crm` program with these arguments
        test_zero_argument = subprocess.run(args)

        # Check if the program returned an error code (non-zero)
        self.assertTrue(test_zero_argument.returncode)

# Run all test cases if the script is executed directly
if __name__ == '__main__':
    unittest.main()
