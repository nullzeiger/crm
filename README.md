# crm

Copy the file to /tmp before removing it

## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Building

Requirements:
- gcc

Requirements for test:
- python3

To build:
```bash
make
```

To install in ~/local/bin:
```bash
make install
```
To uninstall:
```bash
make uninstall
```

## Usage

```bash
./crm --help 
```

Example:
```bash
./crm file.txt    # Remove file add copy in /tmp dir
```

