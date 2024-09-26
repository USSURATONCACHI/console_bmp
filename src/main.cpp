#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstring>

#include <console_bmp/print.hpp>
#include <console_bmp/bmp_reader.hpp>

using console_bmp::println;
using console_bmp::Image;
using console_bmp::BmpReader;



int main(int argc, char** argv) {
    if (argc != 2) {
        println("Error: BMP file is not specified.");
        println("Usage: {} <filepath>", argv[0]);
        println("DrawBMP - reads a BMP image file and prints it to console.");
        return 1;
    }

    std::filesystem::path bmp_filepath(argv[1]);
    println("Filepath: {}", std::string(bmp_filepath));

    std::ifstream ifs(bmp_filepath, std::ios::in | std::ios::binary);

    BmpReader reader;
    std::unique_ptr<Image> image = reader.read_bmp(ifs);

    return 0;
}