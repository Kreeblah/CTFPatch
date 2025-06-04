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

#ifndef CTF_PATCH_H
#define CTF_PATCH_H

#include <stdint.h>

#include "SC55Hashes.h"

#define SC55_STRICT_SC55_COMPAT 1
#define SC55_SC55_COMPAT 2
#define SC55_SC55MKII_COMPAT 4

#define SC55_DRUM_EARLY_COMPAT 1
#define SC55_DRUM_LATE_COMPAT 2

#define SC55_VERIFY_SHA256_CHECKSUM 1
#define SC55_SKIP_VERIFY_SHA256_CHECKSUM 2

typedef struct
{
    size_t rom_size;
    uint8_t *rom_data;
    uint8_t *early_rom_data;
    uint8_t *tone_table;
    uint8_t *drum_table;
    uint8_t *late_rom_data;
    uint8_t rom_sha256[32];
    uint8_t is_known_rom;
    char *rom_name;
    uint8_t *rom_version_address;
} SC55ROMData;

typedef struct
{
    const uint8_t tone_id;
    const char *tone_name;
} SC55TonePair;

typedef struct
{
    const size_t num_tones;
    const SC55TonePair tones[11];
} SC55ToneGroup;

static const SC55ToneGroup SC55_TONES[128] = {
    {3, {{0, "Piano 1"}, {126, "Piano 2"}, {127, "Acou Piano 1"}}},
    {3, {{0, "Piano 2"}, {126, "Piano 2"}, {127, "Acou Piano 2"}}},
    {3, {{0, "Piano 3"}, {126, "Piano 2"}, {127, "Acou Piano 3"}}},
    {3, {{0, "Honky-tonk"}, {126, "Honky-tonk"}, {127, "Elec Piano 1"}}},
    {4, {{0, "E.Piano 1"}, {8, "Detuned EP 1"}, {126, "Piano 1"}, {127, "Elec Piano 2"}}},
    {4, {{0, "E.Piano 2"}, {8, "Detuned EP 2"}, {126, "Piano 2"}, {127, "Elec Piano 3"}}},
    {4, {{0, "Harpsichord"}, {8, "Coupled Hps."}, {126, "Piano 2"}, {127, "Elec Piano 4"}}},
    {3, {{0, "Clav."}, {126, "E.Piano 1"}, {127, "Honkytonk"}}},
    {3, {{0, "Celesta"}, {126, "Detuned EP 1"}, {127, "Elec Org 1"}}},
    {3, {{0, "Glockenspiel"}, {126, "E.Piano 2"}, {127, "Elec Org 2"}}},
    {3, {{0, "Music Box"}, {126, "Steel-str.Gt"}, {127, "Elec Org 3"}}},
    {3, {{0, "Vibraphone"}, {126, "Steel-str.Gt"}, {127, "Elec Org 4"}}},
    {3, {{0, "Marimba"}, {126, "12-str.Gt"}, {127, "Pipe Org 1"}}},
    {3, {{0, "Xylophone"}, {126, "Funk Gt."}, {127, "Pipe Org 2"}}},
    {4, {{0, "Tubular-bell"}, {8, "Church Bell"}, {126, "Muted Gt."}, {127, "Pipe Org 3"}}},
    {3, {{0, "Santur"}, {126, "Slap Bass 1"}, {127, "Accordion"}}},
    {4, {{0, "Organ 1"}, {8, "Detuned Or.1"}, {126, "Slap Bass 1"}, {127, "Harpsi 1"}}},
    {4, {{0, "Organ 2"}, {8, "Detuned Or.2"}, {126, "Slap Bass 1"}, {127, "Harpsi 2"}}},
    {3, {{0, "Organ 3"}, {126, "Slap Bass 1"}, {127, "Harpsi 3"}}},
    {4, {{0, "Church Org.1"}, {8, "Church Org.2"}, {126, "Slap Bass 2"}, {127, "Clavi 1"}}},
    {3, {{0, "Reed Organ"}, {126, "Slap Bass 2"}, {127, "Clavi 2"}}},
    {4, {{0, "Accordion Fr"}, {8, "Accordion It"}, {126, "Slap Bass 2"}, {127, "Clavi 3"}}},
    {3, {{0, "Harmonica"}, {126, "Slap Bass 2"}, {127, "Celesta 1"}}},
    {3, {{0, "Bandneon"}, {126, "Fingered Bs."}, {127, "Celesta 2"}}},
    {4, {{0, "Nylon-str.Gt"}, {8, "Ukulele"}, {126, "Fingered Bs."}, {127, "Syn Brass 1"}}},
    {5, {{0, "Steel-str.Gt"}, {8, "12-str.Gt"}, {16, "Mandolin"}, {126, "Picked Bs."}, {127, "Syn Brass 2"}}},
    {4, {{0, "Jazz Gt."}, {8, "Hawaiian Gt."}, {126, "Picked Bs."}, {127, "Syn Brass 3"}}},
    {4, {{0, "Clean Gt."}, {8, "Chorus Gt."}, {126, "Fretless Bs."}, {127, "Syn Brass 4"}}},
    {4, {{0, "Muted Gt."}, {8, "Funk Gt."}, {126, "Acoustic Bs."}, {127, "Syn Bass 1"}}},
    {3, {{0, "Overdrive Gt"}, {126, "Choir Aahs"}, {127, "Syn Bass 2"}}},
    {4, {{0, "DistortionGt"}, {8, "Feedback Gt."}, {126, "Choir Aahs"}, {127, "Syn Bass 3"}}},
    {4, {{0, "Gt.Harmonics"}, {8, "Gt. Feedback"}, {126, "Choir Aahs"}, {127, "Syn Bass 4"}}},
    {3, {{0, "Acoustic Bs."}, {126, "Choir Aahs"}, {127, "Fantasy"}}},
    {3, {{0, "Fingered Bs."}, {126, "Slow Strings"}, {127, "Harmo Pan"}}},
    {3, {{0, "Picked Bs."}, {126, "Strings"}, {127, "Chorale"}}},
    {3, {{0, "Fretless Bs."}, {126, "Syn.Strings3"}, {127, "Glasses"}}},
    {3, {{0, "Slap Bass 1"}, {126, "Syn.Strings3"}, {127, "Soundtrack"}}},
    {3, {{0, "Slap Bass 2"}, {126, "Organ 1"}, {127, "Atmosphere"}}},
    {4, {{0, "Synth Bass 1"}, {8, "Synth Bass 3"}, {126, "Organ 1"}, {127, "Warm Bell"}}},
    {4, {{0, "Synth Bass 2"}, {8, "Synth Bass 4"}, {126, "Organ 1"}, {127, "Funny Vox"}}},
    {3, {{0, "Violin"}, {126, "Organ 2"}, {127, "Echo Bell"}}},
    {3, {{0, "Viola"}, {126, "Organ 1"}, {127, "Ice Rain"}}},
    {3, {{0, "Cello"}, {126, "Organ 1"}, {127, "Oboe 2001"}}},
    {3, {{0, "Contrabass"}, {126, "Organ 2"}, {127, "Echo Pan"}}},
    {3, {{0, "Tremolo Str"}, {126, "Organ 2"}, {127, "Doctor Solo"}}},
    {3, {{0, "PizzicatoStr"}, {126, "Organ 2"}, {127, "School Daze"}}},
    {3, {{0, "Harp"}, {126, "Trumpet"}, {127, "Bellsinger"}}},
    {3, {{0, "Timpani"}, {126, "Trumpet"}, {127, "Square Wave"}}},
    {4, {{0, "Strings"}, {8, "Orchestra"}, {126, "Trombone"}, {127, "Str Sect 1"}}},
    {3, {{0, "Slow Strings"}, {126, "Trombone"}, {127, "Str Sect 2"}}},
    {4, {{0, "Syn.Strings1"}, {8, "Syn.Strings3"}, {126, "Trombone"}, {127, "Str Sect 3"}}},
    {3, {{0, "Syn.Strings2"}, {126, "Trombone"}, {127, "Pizzicato"}}},
    {3, {{0, "Choir Aahs"}, {126, "Trombone"}, {127, "Violin 1"}}},
    {3, {{0, "Voice Oohs"}, {126, "Trombone"}, {127, "Violin 2"}}},
    {3, {{0, "SynVox"}, {126, "Alto Sax"}, {127, "Cello 1"}}},
    {3, {{0, "OrchestraHit"}, {126, "Tenor Sax"}, {127, "Cello 2"}}},
    {3, {{0, "Trumpet"}, {126, "Baritone Sax"}, {127, "Contrabass"}}},
    {3, {{0, "Trombone"}, {126, "Alto Sax"}, {127, "Harp 1"}}},
    {3, {{0, "Tuba"}, {126, "Brass 1"}, {127, "Harp 2"}}},
    {3, {{0, "MutedTrumpet"}, {126, "Brass 1"}, {127, "Guitar 1"}}},
    {3, {{0, "French Horn"}, {126, "Brass 2"}, {127, "Guitar 2"}}},
    {4, {{0, "Brass 1"}, {8, "Brass 2"}, {126, "Brass 2"}, {127, "Elec Gtr 1"}}},
    {4, {{0, "Synth Brass1"}, {8, "Synth Brass3"}, {126, "Brass 1"}, {127, "Elec Gtr 2"}}},
    {4, {{0, "Synth Brass2"}, {8, "Synth Brass4"}, {126, "OrchestraHit"}, {127, "Sitar"}}},
    {2, {{0, "Soprano Sax"}, {127, "Acou Bass 1"}}},
    {2, {{0, "Alto Sax"}, {127, "Acou Bass 2"}}},
    {2, {{0, "Tenor Sax"}, {127, "Elec Bass 1"}}},
    {2, {{0, "Baritone Sax"}, {127, "Elec Bass 2"}}},
    {2, {{0, "Oboe"}, {127, "Slap Bass 1"}}},
    {2, {{0, "English Horn"}, {127, "Slap Bass 2"}}},
    {2, {{0, "Bassoon"}, {127, "Fretless 1"}}},
    {2, {{0, "Clarinet"}, {127, "Fretless 2"}}},
    {2, {{0, "Piccolo"}, {127, "Flute 1"}}},
    {2, {{0, "Flute"}, {127, "Flute 2"}}},
    {2, {{0, "Recorder"}, {127, "Piccolo 1"}}},
    {2, {{0, "Pan Flute"}, {127, "Piccolo 2"}}},
    {2, {{0, "Bottle Blow"}, {127, "Recorder"}}},
    {2, {{0, "Shakuhachi"}, {127, "Pan Pipes"}}},
    {2, {{0, "Whistle"}, {127, "Sax 1"}}},
    {2, {{0, "Ocarina"}, {127, "Sax 2"}}},
    {3, {{0, "Square Wave"}, {8, "Sine Wave"}, {127, "Sax 3"}}},
    {2, {{0, "Saw Wave"}, {127, "Sax 4"}}},
    {2, {{0, "Syn.Calliope"}, {127, "Clarinet 1"}}},
    {2, {{0, "Chiffer Lead"}, {127, "Clarinet 2"}}},
    {2, {{0, "Charang"}, {127, "Oboe"}}},
    {2, {{0, "Solo Vox"}, {127, "Engl Horn"}}},
    {2, {{0, "5th Saw Wave"}, {127, "Bassoon"}}},
    {2, {{0, "Bass & Lead"}, {127, "Harmonica"}}},
    {2, {{0, "Fantasia"}, {127, "Trumpet 1"}}},
    {2, {{0, "Warm Pad"}, {127, "Trumpet 2"}}},
    {2, {{0, "Polysynth"}, {127, "Trombone 1"}}},
    {2, {{0, "Space Voice"}, {127, "Trombone 2"}}},
    {2, {{0, "Bowed Glass"}, {127, "Fr Horn 1"}}},
    {2, {{0, "Metal Pad"}, {127, "Fr Horn 2"}}},
    {2, {{0, "Halo Pad"}, {127, "Tuba"}}},
    {2, {{0, "Sweep Pad"}, {127, "Brs Sect 1"}}},
    {2, {{0, "Ice Rain"}, {127, "Brs Sect 2"}}},
    {2, {{0, "Soundtrack"}, {127, "Vibe 1"}}},
    {2, {{0, "Crystal"}, {127, "Vibe 2"}}},
    {2, {{0, "Atmosphere"}, {127, "Syn Mallet"}}},
    {2, {{0, "Brightness"}, {127, "Windbell"}}},
    {2, {{0, "Goblin"}, {127, "Glock"}}},
    {2, {{0, "Echo Drops"}, {127, "Tube Bell"}}},
    {2, {{0, "Star Theme"}, {127, "Xylophone"}}},
    {2, {{0, "Sitar"}, {127, "Marimba"}}},
    {2, {{0, "Banjo"}, {127, "Koto"}}},
    {2, {{0, "Shamisen"}, {127, "Sho"}}},
    {3, {{0, "Koto"}, {8, "Taisho Koto"}, {127, "Shakuhachi"}}},
    {2, {{0, "Kalimba"}, {127, "Whistle 1"}}},
    {2, {{0, "Bag Pipe"}, {127, "Whistle 2"}}},
    {2, {{0, "Fiddle"}, {127, "Bottleblow"}}},
    {2, {{0, "Shanai"}, {127, "Breathpipe"}}},
    {2, {{0, "Tinkle Bell"}, {127, "Timpani"}}},
    {2, {{0, "Agogo"}, {127, "Melodic Tom"}}},
    {2, {{0, "Steel Drums"}, {127, "Deep Snare"}}},
    {3, {{0, "Woodblock"}, {8, "Castanets"}, {127, "Elec Perc 1"}}},
    {3, {{0, "Taiko"}, {8, "Concert BD"}, {127, "Elec Perc 2"}}},
    {3, {{0, "Melo. Tom 1"}, {8, "Melo. Tom 2"}, {127, "Taiko"}}},
    {3, {{0, "Synth Drum"}, {8, "808 Tom"}, {127, "Taiko Rim"}}},
    {2, {{0, "Reverse Cym."}, {127, "Cymbal"}}},
    {4, {{0, "Gt.FretNoise"}, {1, "Gt.Cut Noise"}, {2, "String Slap"}, {127, "Castanets"}}},
    {3, {{0, "Breath Noise"}, {1, "Fl.Key Click"}, {127, "Triangle"}}},
    {7, {{0, "Seashore"}, {1, "Rain"}, {2, "Thunder"}, {3, "Wind"}, {4, "Stream"}, {5, "Bubble"}, {127, "Orche Hit"}}},
    {4, {{0, "Bird"}, {1, "Dog"}, {2, "Horse-Gallop"}, {127, "Telephone"}}},
    {7, {{0, "Telephone 1"}, {1, "Telephone 2"}, {2, "DoorCreaking"}, {3, "Door"}, {4, "Scratch"}, {5, "Windchime"}, {127, "Bird Tweet"}}},
    {11,{{0, "Helicopter"}, {1, "Car-Engine"}, {2, "Car-Stop"}, {3, "Car-Pass"}, {4, "Car-Crash"}, {5, "Siren"}, {6, "Train"}, {7, "Jetplane"}, {8, "Starship"}, {9, "Burst Noise"}, {127, "One Note Jam"}}},
    {7, {{0, "Applause"}, {1, "Laughing"}, {2, "Screaming"}, {3, "Punch"}, {4, "Heart Beat"}, {5, "Footsteps"}, {127, "Water Bell"}}},
    {5, {{0, "Gun Shot"}, {1, "Machine Gun"}, {2, "Lasergun"}, {3, "Explosion"}, {127, "Jungle Tune"}}}
};

SC55ROMData ParseROM(uint8_t *rom_data, size_t rom_size, uint8_t ignore_sha256_failures);

void DestroyROM(SC55ROMData *rom);

SC55ROMData ReadROM(const char *rom_file_path, uint8_t ignore_sha256_failures);

int WriteROM(const SC55ROMData *rom, const char *rom_file_path);

SC55Hash IdentifyROM(const uint8_t rom_sha256[32], size_t rom_size);

int PatchROM(SC55ROMData *rom, uint8_t compat_mode, uint8_t drum_compat_mode, uint8_t update_version);

#endif /* CTF_PATCH_H */
