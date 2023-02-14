# sarc-differ
Simple command-line diff tool for Nintendo EPD "Module System" titles. Supports Zstd and SZS decompression, Nintendo EPD's "sarc" and "bars" 1.2 formats, and the NintendoWare "bntx" 4.1.0, "bea" 1.1.0, and partial support for "bfres" 10.0.0 archives as well as "versioned paths".

Usage: "sarcdiff.exe [left root romfs directory] [right root romfs directory]"

Prints diff to stdout. For file output: "sardiff.exe [left root romfs directory] [right root romfs directory] > [your_file_name.txt]"

Built with GCC 12.2 and zstd from msys2. After installing both run "make" from the command-line in the projects root directory.
