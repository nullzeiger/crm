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

import unittest
import subprocess
import os

class TestCRM(unittest.TestCase):
    def test_file_removal(self):
        args = ['./crm', 'test.txt']

        # Create file test.txt
        file = open(args[1], 'w')
        file.close()

        # Call ./crm test.txt
        subprocess.run(args)

        # Check if file test.txt exists in /tmp directory
        self.assertTrue(os.path.exists(f"/tmp/{args[1]}"))

        # Check if file test.txt exists in root directory
        self.assertFalse(os.path.exists(args[1]))

    def test_more_arguments(self):
        args = ['./crm', 'hello', 'world']

        # Call ./crm test.txt
        test_more_arguments = subprocess.run(args)

        # Check multi arguments
        self.assertTrue(test_more_arguments.returncode)

    def test_zero_argument(self):
        args = './crm'

        # Call ./crm test.txt
        test_zero_argument = subprocess.run(args)

        # Check zero arguments
        self.assertTrue(test_zero_argument.returncode)

if __name__ == '__main__':
    unittest.main()
