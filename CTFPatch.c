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

#include "CTFPatch.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LONESHA256_STATIC
#include "lonesha256.h"

#include "SC55Hashes.h"

SC55ROMData ParseROM(uint8_t *rom_data, const size_t rom_size, const uint8_t ignore_sha256_failures)
{
    SC55ROMData rom;
    rom.rom_size = 0;
    rom.rom_data = NULL;
    rom.early_rom_data = NULL;
    rom.tone_table = NULL;
    rom.drum_table = NULL;
    rom.late_rom_data = NULL;
    memset(rom.rom_sha256, 0, 32);
    rom.is_known_rom = 0;
    rom.rom_name = NULL;
    rom.rom_version_address = 0;

    if(rom_data == NULL || rom_size < 0x38080)
    {
        return rom;
    }

    if(lonesha256(rom.rom_sha256, rom_data, rom_size) > 0)
    {
        memset(rom.rom_sha256, 0, 32);
        return rom;
    }

    rom.rom_size = rom_size;
    rom.rom_data = rom_data;
    rom.early_rom_data = rom_data;
    rom.tone_table = rom_data + 0x30000;
    rom.drum_table = rom_data + 0x38000;
    rom.late_rom_data = rom_data + 0x38080;

    const SC55Hash rom_hash = IdentifyROM(rom.rom_sha256, rom.rom_size);
    if(rom_hash.file_size == 0 && ignore_sha256_failures == 0)
    {
        DestroyROM(&rom);
        return rom;
    }

    if(rom_hash.file_size != 0)
    {
        rom.is_known_rom = 1;
        rom.rom_name = (char*)rom_hash.rom_name;
        rom.rom_version_address = rom.rom_data + rom_hash.version_address;
    }

    return rom;
}

void DestroyROM(SC55ROMData *rom)
{
    rom->rom_size = 0;
    if(rom->rom_data != NULL)
    {
        free(rom->rom_data);
    }
    rom->rom_data = NULL;
    rom->early_rom_data = NULL;
    rom->tone_table = NULL;
    rom->drum_table = NULL;
    rom->late_rom_data = NULL;
    memset(rom->rom_sha256, 0, 32);
    rom->is_known_rom = 0;
    rom->rom_name = NULL;
    rom->rom_version_address = NULL;
}

SC55ROMData ReadROM(const char *rom_file_path, const uint8_t ignore_sha256_failures)
{
    FILE *fp;

    int seek_status = 0;
    uint8_t *rom_data = NULL;
    size_t rom_size;

    SC55ROMData rom;
    rom.rom_size = 0;
    rom.rom_data = NULL;
    rom.early_rom_data = NULL;
    rom.tone_table = NULL;
    rom.drum_table = NULL;
    rom.late_rom_data = NULL;
    memset(rom.rom_sha256, 0, 32);
    rom.is_known_rom = 0;
    rom.rom_name = NULL;
    rom.rom_version_address = NULL;

    if(!rom_file_path)
    {
        return rom;
    }

    fp = fopen(rom_file_path, "rb");
    if(fp == NULL)
    {
        return rom;
    }

    seek_status = fseek(fp, 0L, SEEK_END);

    if(seek_status != 0)
    {
        fclose(fp);
        return rom;
    }
    else
    {
        rom_size = (size_t)ftell(fp);
        fseek(fp, 0L, SEEK_SET);
    }

    rom_data = (uint8_t*)malloc(rom_size);

    if(rom_data == NULL)
    {
        fclose(fp);
        return rom;
    }

    fread(rom_data, 1, rom_size, fp);
    fclose(fp);

    rom = ParseROM(rom_data, rom_size, ignore_sha256_failures);
    if(rom.rom_size == 0 || rom.rom_data == NULL)
    {
        DestroyROM(&rom);
        return rom;
    }

    return rom;
}

int WriteROM(const SC55ROMData *rom, const char *rom_file_path)
{
    FILE *fp;
    size_t bytes_written = 0;

    if(!rom || !rom_file_path)
    {
        return 1;
    }

    fp = fopen(rom_file_path, "wb");
    if(!fp)
    {
        return 1;
    }

    bytes_written = fwrite(rom->rom_data, 1, rom->rom_size, fp);
    fclose(fp);

    if(bytes_written != rom->rom_size)
    {
        return 1;
    }

    return 0;
}

SC55Hash IdentifyROM(const uint8_t rom_sha256[32], const size_t rom_size)
{
    char rom_sha256_str[65];
    uint8_t i;

    size_t sc55_num_hashes;

    for(i = 0; i < 32; i++)
    {
        sprintf(rom_sha256_str + (2 * i), "%02x", rom_sha256[i]);
    }

    rom_sha256_str[64] = 0;
    sc55_num_hashes = sizeof(SC55_HASHES)/sizeof(SC55Hash);

    for(i = 0; i < sc55_num_hashes; i++)
    {
        if(strcmp(SC55_HASHES[i].sha256hash, rom_sha256_str) == 0 && SC55_HASHES[i].file_size == rom_size)
        {
            return SC55_HASHES[i];
        }
    }

    return (SC55Hash){"", 0, "", 0};
}

uint8_t SubcapitalToneExists(const size_t bank, const uint8_t tone)
{
    size_t i;

    for(i = 0; i < SC55_TONES[bank].num_tones; i++)
    {
        if(SC55_TONES[bank].tones[i].tone_id == tone)
        {
            return 1;
        }
    }

    return 0;
}

uint16_t GetSubcapitalTone(const uint16_t tone_table[128][128], const size_t prog, const size_t bank, const uint8_t compat_mode)
{
    uint16_t subcapital_tone = tone_table[bank & 0x78][prog];
    uint8_t table_subcapital_tone_exists = 0;

    if(compat_mode == SC55_STRICT_SC55_COMPAT || compat_mode == SC55_SC55_COMPAT)
    {
        table_subcapital_tone_exists = SubcapitalToneExists(prog, bank);

        if(!table_subcapital_tone_exists)
        {
            subcapital_tone = 0xffff;
        }
    }

    return subcapital_tone;
}

int PatchROM(SC55ROMData *rom, const uint8_t compat_mode, const uint8_t drum_compat_mode, const uint8_t update_version)
{
    size_t bank, prog, i;
    uint16_t tone_table[128][128];

    uint8_t new_tone_table[0x8000];
    size_t table_index = 0;

    size_t drum_prog_threshold;
    uint8_t drum_patch_value = 0;

    uint16_t table_tone = 0xffff;
    uint16_t current_tone = 0xffff;
    uint16_t updated_tone = 0xffff;

    for(bank = 0; bank < 128; bank++)
    {
        for(prog = 0; prog < 128; prog++)
        {
            table_tone = (uint16_t)(rom->tone_table[((bank * 128) + prog) * 2]) << 8;
            table_tone = table_tone | (uint16_t)(rom->tone_table[(((bank * 128) + prog) * 2) + 1]);
            tone_table[bank][prog] = table_tone;
        }
    }

    for(i = 0; i < 0x8000; i++)
    {
        new_tone_table[i] = rom->tone_table[i];
    }

    /* Banks 64 and up are for special use and excluded */
    for(bank = 0; bank < 64; bank++)
    {
        for(prog = 0; prog < 128; prog++)
        {
            current_tone = tone_table[bank][prog];

            /* Tones 121 (120 from a 0-indexed language like C) and up are sound effects and excluded */
            if(prog < 120 && (current_tone == 0xffff || (compat_mode == SC55_STRICT_SC55_COMPAT && !SubcapitalToneExists(prog, bank))))
            {
                updated_tone = GetSubcapitalTone(tone_table, prog, bank, compat_mode);
                if(updated_tone == 0xffff)
                {
                    updated_tone = tone_table[0][prog];
                }
            }
            else
            {
                updated_tone = current_tone;
            }

            new_tone_table[table_index] = (uint8_t)(updated_tone >> 8);
            new_tone_table[table_index + 1] = (uint8_t)(updated_tone & 0xff);

            table_index += 2;
        }
    }

    for(i = 0; i < 0x8000; i++)
    {
        rom->tone_table[i] = new_tone_table[i];
    }

    if(drum_compat_mode == SC55_DRUM_EARLY_COMPAT)
    {
        drum_prog_threshold = 64;
    }
    else
    {
        drum_prog_threshold = 48;
    }

    for(i = 0; i < drum_prog_threshold; i++)
    {
        if(i % 8 == 0)
        {
            drum_patch_value = rom->drum_table[i];
        }

        if(rom->drum_table[i] == 0xff)
        {
            rom->drum_table[i] = drum_patch_value;
        }
    }

    if(update_version && rom->is_known_rom)
    {
        rom->rom_version_address[2] = 'C';
        rom->rom_version_address[3] = 'T';
    }

    return 0;
}
