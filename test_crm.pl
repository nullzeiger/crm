#!/usr/bin/env perl

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

use strict;
use warnings;

use English '-no_match_vars';
use Test::More tests  => 3;

# Tests whether the file was copied to the /tmp directory and removed to the root
sub file_removal {
  my ( $command, $argument ) = @_;

  # Create file test.txt.
  open my $fh, '>', $argument or die "Error create file: $OS_ERROR\n";
  close $fh or die "unable to close $OS_ERROR\n";

  # Call ./crm test.txt.
  system ($command, $argument);

  my $file_exists = 0;

  # Check if file test.txt exixts in /tmp directory.
  if (-e "/tmp/$argument") {
    $file_exists = 1;
  }

  # Check if file test.txt exixts in root directory.
  if (-e "$argument") {
    $file_exists = 0;
  }

  $file_exists
}

# Test how the program works with one more argument than expected
sub using_more_arguments {
  my @args = @_;
  system @args;
}

# Test the program's operation with zero arguments
sub using_zero_argument {
  my ( $command ) = @_;
  system $command;
}

# To get the actual exit value, shift right by eight
sub exit_code {
  my $value = shift;
  $value >> 8;
}

# Run all tests
sub exec_tests {
  my @args = qw ( ./crm test.txt more );

  ok(exit_code(file_removal @args) == 0, './crm remove.txt');
  ok(exit_code(using_more_arguments @args) == 1, './crm hello world');
  ok(exit_code(using_zero_argument @args) == 1, './crm');

  # test done.
  done_testing;
}

# Exec all tests
exec_tests
