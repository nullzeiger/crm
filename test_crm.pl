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

use Test::More;

my @args = qw(./crm test.txt);

# Create file test.txt.
open my $fh, '>', $args[1];
close $fh;

# Call ./crm test.txt.
system(@args) == 0 or die "system @args failed: $?";

my $file_exists = 0;

# Check if file test.txt exixts in /tmp directory.
if (-e "/tmp/$args[1]") {
  $file_exists = 1;
}

# Check if file test.txt exixts in root directory.
if (-e "$args[1]") {
  $file_exists = 0;
}

# Exec test.
ok($file_exists, "pass");

# test done.
done_testing();
