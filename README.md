# libby
Command-line utility for Shadowcaster (DOS). Extract or replace the contents of .lib files!

## Usage
`libby [-option] [infile] [lumpname] [outfile]`

-x : extract a lump from infile, outfile is written. lumpname is the name of the lump you want to extract.

-r : replace lump in infile with the file lumpname, then write outfile.

## Building
Built with mingw32 in mind, but it should be sufficient on any build system to just invoke `make` in the source directory and get binaries in `\bin`.
