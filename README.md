# sarc-differ
Command-line diff tool for Nintendo EPD games built using that one game engine (ModuleSystem?) used in Splatoon 3 and Nintendo Switch Sports

Supports:
* Zstd and SZS decompression
* Nintendo's sarc archive format
* Nintendo EPD's byaml data interchange format up through v7
* Nintendo EPD's bars v1.2 audio resource archive format
* NintendoWare bntx v4.1.0 texture archive format
* NintendoWare bea v1.1.0 Bezel Engine archive format
* NintendoWare bfres v10 model resource format

Usage: "sarcdiff.exe [left root of romfs directory] [right root of romfs directory]"

Prints diff to stdout. For file output: "sarcdiff.exe [left root romfs directory] [right root romfs directory] > [your_file_name.txt]"

Built with GCC 12.2 and zstd from msys2. After installing both run "make" from the command-line in the projects root directory.
