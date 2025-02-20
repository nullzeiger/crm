# Copyright (C) 2025 Ivan Guerreschi <ivan.guerreschi.dev@gmail.com>.
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

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -ggdb

# Directories
SRC_DIR = .

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=%.o)

# Binary name
TARGET = crm

# Phony targets
.PHONY: all clean dirs

# Default target
all: dirs $(TARGET)

# Link the program
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf *.o $(TARGET)

# Install target (optional)
install: all
	install -m 755 $(TARGET) ~/.local/bin

# Uninstall target (optional)
uninstall: all
	rm -rf ~/.local/$(TARGET)

# Test using python script
test: all
	@python3 crm_test.py
