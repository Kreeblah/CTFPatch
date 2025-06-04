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

#ifndef SC55HASHES_H
#define SC55HASHES_H

#include <stddef.h>

typedef struct
{
    const char sha256hash[64];
    const size_t file_size;
    const char rom_name[128];
    const size_t version_address;
} SC55Hash;

static const SC55Hash SC55_HASHES[] = {
    {"a4c9fd821059054c7e7681d61f49ce6f42ed2fe407a7ec1ba0dfdc9722582ce0", 524288, "SC-55 mkII 1.01", 0xfff0},
    {"2e138479101452f7e331f95dd7be9232df6dda317c8fd38b6d71243c210c5e6d", 524288, "XP-10 1.02", 0x4405d}
};

#endif /* SC55HASHES_H */
