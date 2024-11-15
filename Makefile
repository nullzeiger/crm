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

crm: crm.c main.c
	gcc -Wall -Werror -ggdb -fanalyzer -O2 -o crm crm.c main.c

clean:
	rm crm

test:
	@python3 test_crm.py

indent:
	indent -kr -i8 crm.c crm.h main.c

# for emacs flymake
.PHONY:
	check-syntax
check-syntax:
	gcc -Wall -Werror -o /dev/null -S $(CHK_SOURCES) || true
