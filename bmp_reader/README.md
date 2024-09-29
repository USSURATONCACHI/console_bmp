# That is no the CLI app itself. Go to `../console_bmp/` folder for the app.

# bmp_reader 
## (`#include <bmp_reader/bmp_reader.hpp>`)

Its a library that abstracts away all the different standards of BMP file format.

There are so many classes only because there are so many different edge cases with all the standards.

## BMP types.

BMP has 8 existing variants of the standard: 

(old)
- BITMAP ***CORE*** HEADER
- ***OS21X*** BITMAPHEADER

(modern)
- BITMAP ***INFO*** HEADER
- ***OS22X*** BITMAPHEADER
- BITMAP ***V2INFO*** HEADER
- BITMAP ***V3INFO*** HEADER
- BITMAP ***V4*** HEADER
- BITMAP ***V5*** HEADER

Folder `dib_headers/` contains classes for Headers and HeaderParsers for 4 of 8 types.

Folder `readers/` contains classes for parsing the image data itself based on headers.