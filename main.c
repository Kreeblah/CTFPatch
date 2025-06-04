/*
** Copyright 2025 Christopher Gelatt
**
** This program is free software: you can redistribute it
** and/or modify it under the terms of the
** GNU Lesser General Public License as published by the
** Free Software Foundation, either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be
** useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the
** GNU Lesser General Public License along with this program.
** If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CTFPatch.h"

void print_help(void);
int process_rom(const char *input_rom_path, const char *output_rom_path, uint8_t sc55_compat_mode, uint8_t sc55_drum_compat_mode, uint8_t ignore_checksum, uint8_t update_version);

int main(int argc, char **argv)
{
    char* rom_input_path = NULL;
	char* rom_output_path = NULL;
	uint8_t ignore_checksum = 0;
	uint8_t sc55_compat_mode = 2;
	uint8_t sc55_drum_compat_mode = 1;
	uint8_t update_version = 1;
	int c;
	int operation_result = 0;

	while((c = getopt(argc, argv, "i:o:cs:d:vh")) != -1)
	{
		switch(c)
		{
			case 'i':
				rom_input_path = strdup(optarg);
				break;
			case 'o':
				rom_output_path = strdup(optarg);
				break;
			case 'c':
				ignore_checksum = 1;
				break;
			case 's':
				if(strcmp(optarg, "strict") == 0)
				{
					sc55_compat_mode = 1;
					break;
				}
				if(strcmp(optarg, "sc55") == 0)
				{
					sc55_compat_mode = 2;
					break;
				}
				if(strcmp(optarg, "mkii") == 0)
				{
					sc55_compat_mode = 4;
					break;
				}
				print_help();
				exit(1);
			case 'd':
				if(strcmp(optarg, "early") == 0)
				{
					sc55_drum_compat_mode = 1;
					break;
				}
				if(strcmp(optarg, "late") == 0)
				{
					sc55_drum_compat_mode = 2;
					break;
				}
				print_help();
				exit(1);
			case 'v':
				update_version = 0;
				break;
			case 'h':
			default:
				print_help();
				exit(0);
		}
	}

	if(!rom_input_path || !rom_output_path)
	{
		print_help();
		exit(1);
	}

	operation_result = process_rom(rom_input_path, rom_output_path, sc55_compat_mode, sc55_drum_compat_mode, ignore_checksum, update_version);

	exit(operation_result);
}

int process_rom(const char *input_rom_path, const char *output_rom_path, const uint8_t sc55_compat_mode, const uint8_t sc55_drum_compat_mode, const uint8_t ignore_checksum, const uint8_t update_version)
{
	int operation_result = 0;

	SC55ROMData rom_data;

	printf("Reading ROM...\n");
	rom_data = ReadROM(input_rom_path, ignore_checksum);
	if(!rom_data.rom_data)
	{
		printf("Unable to read ROM data from %s\n", input_rom_path);
		printf("Verify that the ROM is a supported image.\n");
		return 1;
	}

	printf("ROM data read.  Patching...\n");

	operation_result = PatchROM(&rom_data, sc55_compat_mode, sc55_drum_compat_mode, update_version);
	if(operation_result)
	{
		printf("Error applying ROM patch.\n");
		return operation_result;
	}

	printf("ROM data patched.  Writing...\n");

	operation_result = WriteROM(&rom_data, output_rom_path);
	if(operation_result)
	{
		printf("Error writing ROM to %s\n", output_rom_path);
		return operation_result;
	}

	printf("ROM written successfully.\n");

	DestroyROM(&rom_data);
	return 0;
}

void print_help(void)
{
	printf("Usage: CTFPatch [options] -i [FILE] -o [FILE]\n");
	printf("Options:\n");
	printf("  -i FILE  Path to input ROM\n");
	printf("  -o FILE  Path to output ROM\n");
	printf("  -c       If set, will ignore unknown checksums\n");
	printf("  -s ARG   Which SC-55 compatibility mode to use\n");
	printf("           Valid values: strict sc55 mkii\n");
	printf("           Defaults to sc55 if unset\n");
	printf("  -d ARG   Which drum compatibility mode to use\n");
	printf("           Valid values: early late\n");
	printf("           Defaults to early if unset\n");
	printf("  -v       Do not update ROM version string\n");
	printf("           for known checksums.\n");
	printf("  -h       Display this information\n");
	printf("\n");
	printf("Notes:\n");
	printf("-i and -o are required\n");
	return;
}
