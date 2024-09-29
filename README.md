# console_bmp

Its a cross-platform CLI viewer for .bmp . Supports all uncompressed BMP types.

Entry point is `console_bmp/src/main.cpp`. The program is `build/bin/Debug/console_cpp.exe` (if MSBuild) or `build/bin/console_bmp.exe` (with other toolchains).

- `$ console_bmp sample_images/black_and_white_1_16x48_px.bmp`
    ![Simplest usage](readme_images/simplest_usage.png)

- `$ console_bmp --gradient sample_images/blackbuck.bmp`:
    ![Blackbuck](readme_images/blabkbuck_gradient.png)

## Building

Build is done with CMake:
```bash
$ git submodule update --init --recursive
$ cmake -S . -B build
$ cmake --build build --parallel
$ cd build/bin # Might be different with MSBuild
```

Building produces two executables: `console_bmp` and `run_tests`.
Use `run_tests` to check that program works correctly as expected.

You can specify cmake option `SFML_WINDOW_DISPLAY` to have a `--window` cli feature that opens image in a window.
Here is how it is used from CLI:
![Window feature usage](readme_images/sfml_window_example.png)

It is specified like this before build:
```bash
$ cmake -S . -B build -DSFML_WINDOW_DISPLAY=ON
```

## Layout

- `console_bmp/` - the CLI app itself.
- `bmp_reader/` - library that abstract away parsing BMP files. Reads files via `std::istream&`.
- `tests/` - module that produces `run_tests.exe` to test correctness of the library.

## Usage

1. Just print whole image to terminal in 1-bit mode:
    ``` bash
    $ ./console_bmp bmp_8x24_px.bmp 

    $$$$$$$$$$$$$$$$$ $$$$$$
    $ $$$$$$$$$$$$$$   $$ $$
    $ $$$$$$$$$$$$$ $ $ $ $$
    $ $$$ $$$$$$$$$$$ $ $$ $
    $   $     $   $$   $$$$ 
    $ $ $ $ $ $   $ $ $$$$ $
    $   $ $ $ $ $$$ $ $ $ $$
    $$$$$$$$$$$$$$$$   $$ $$
    ```

2. `--no-raw-text` - do not print image to terminal (used with other flags):
    ```bash
    $ ./console_bmp --no-raw-text bmp_8x24_px.bmp
    $
    ```

2. `--info`, `-i` - show info about image:
    ```bash
    $ ./console_bmp --no-raw-text --info bmp_8x24_px.bmp
    BITMAPINFOHEADAER[v1-v5]
    Image size: 24 8
    Bits per pixel: 24
    Compression method: RGB
    Raw data size: 576
    Num colors in palette: 0
    Important colors: 0
    ```
3. `-w`, `-h`, `--auto-width` - set MAXIMUM size of output image. 
    `-w` accepts maximum width in symbols, `-h` - maximum size.
    `--auto-width` sets the maximum size to the size of your terminal.

    ```bash
    $ ./console_bmp -w 5 ./blackbuck.bmp 

      $  
     $$  
      $$$
       $$

    $ ./console_bmp -w 15 ./blackbuck.bmp 
                   
                   
           $$      
          $$       
        $$$$       
        $$$$       
        $$$$       
      $$$$$$$   $$$
       $$ $$$$$$$$$
           $$$$$$$$
            $$$$$$$
            $$$$$$$
             $$$$$$
              $$$$$
    ```

4. `--gradient` - use more than two colors:

5. Build flag OPTIONAL: `--window`. Opens SFML window with specified image in RGBA8. 