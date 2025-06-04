# CTFPatch
Patch capital tone fallback into SC-55 mkII ROMs

Based on [shingo45endo](https://github.com/shingo45endo)'s work on a [Javascript implementation](https://github.com/shingo45endo/sc55mk2-ctf-patcher) to perform similar tasks.

Uses a modified version of [lonesha256](https://github.com/BareRose/lonesha256) under a CC0 1.0 license.

# Purpose
CTFPatch is a portable, straightforward, linkable project for reading known SC-55 ROMs, patching them with capital tone fallback tables, and writing them back out to disk.  Reading and writng the files is optional, and those functions can be performed outside of this code, allowing for other applications to use the libraries built from this code for live-patching in-memory ROM data.

# Building
Building should be straightforward for any platform with a usable `make` utility.  The makefile will produce a static library, a shared library, and a standalone, statically-linked binary.  C99 and newer language standards are supported.

# Usage
To run CTFPatch, you need to provide at minimum two things: the location of the source ROM and the location of the output file to be created by the utility.  Full options are:

```
Usage: CTFPatch [options] -i [FILE] -o [FILE]
Options:
  -i FILE  Path to input ROM
  -o FILE  Path to output ROM
  -c       If set, will ignore unknown checksums
  -s ARG   Which SC-55 compatibility mode to use
           Valid values: strict sc55 mkii
           Defaults to sc55 if unset
  -d ARG   Which drum compatibility mode to use
           Valid values: early late
           Defaults to early if unset
  -v       Do not update ROM version string
           for known checksums.
  -h       Display this information

Notes:
-i and -o are required
```

Note that by default, unknown ROMs will be rejected.  Currently known ROMs are the SC-55 mkII 1.01 ROM and the XP-10 1.02 ROM.  These ROMs are detected via SHA256 hash, as provided by `lonesha256`.

Additionally, by default, the last two characters of the version string in the ROM are replaced with `CT`.  So, for example, the version string of an SC-55 1.01 ROM would be changed to `1.CT`.  This is to easily identify that these ROMs have been patched with capital tone fallback data and are not original ROMs when the version screen is shown on a real synthesizer.

# Library usage
The methods here are pretty straightforward.  `ReadROM()` will read a ROM file from disk and parse it, `ParseROM()` will parse in-memory ROM data.  `IdentifyROM()` will return a struct with information about a known ROM based on its SHA256 checksum.  `PatchROM()` will apply in-memory patches.  `WriteROM()` will write the ROM file to disk.  `DestroyROM()` clears the ROM from memory and sets pointers back to `NULL`.

# Types of compatibility patches

There are three types of compatibility patches for instruments, and two for drums.

For instruments:

- Strict SC-55 compatibility means that tones which are not in the original SC-55 but do exist in the ROM being patched are set to fall back to what they would have been with the SC-55.
- SC-55 compatibility mode adds fallback tones where they are missing based on the original SC-55's mapping, but keeps tones which have been added in the version of the ROM being patched.  This is the default, for best compatibility.
- SC-55 mkII compatibility mode only adds fallback data, but only to the base tones used by the SC-55 mkII.  The rest of the tones from the SC-55 aren't mapped.

For drums:

- If early mode is used, the drum table from the SC-55 version 1.21 and below is patched in.  This is the default, as it provides the best compatibility.
- If late mode is used, the drum table from the SC-55 version 2.00 and later is patched in.
