#include <bmp_reader/util/print.hpp>
#include <console_bmp/cli_args.hpp>
#include <console_bmp/bmp_display.hpp>

// #define GRADIENT " .'`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"

using bmp_reader::println;
using bmp_reader::eprintln;

using namespace console_bmp;

int main(int argc, char** argv) {
    AppArguments cli_args; 
    
    // Parse args
    try {
        cli_args = parse_args(argc, argv);
    } catch (const std::exception& e) {
        eprintln("Failed to parse arguments: {}", e.what());
        return 1;
    }

    println("Skibidi sigma!");
    BmpDisplay display;
    
    display.openBMP(cli_args.bmp_file_path.string());
    display.displayBMP();
    display.closeBMP();

    return 0;
}

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         println("Error: BMP file is not specified.");
//         println("Usage: {} <filepath>", argv[0]);
//         println("DrawBMP - reads a BMP image file and prints it to console.");
//         return 1;
//     }

//     std::filesystem::path bmp_filepath(argv[1]);
//     println("Filepath: {}", std::string(bmp_filepath));

//     std::ifstream ifs(bmp_filepath, std::ios::in | std::ios::binary);
//     if (!ifs.is_open()) {
//         println("Failed to open the file.");
//         return 2;
//     }

//     BmpReader reader;
//     std::unique_ptr<Image> image = reader.read_bmp(ifs);

//     println("Image size: {} x {}", image->width(), image->height());

//     std::ofstream ofs("imgdump.bin", std::ios::out | std::ios::binary);
//     auto width = static_cast<uint32_t>(image->width()) ;
//     auto height = static_cast<uint32_t>(image->height());
//     ofs.write(reinterpret_cast<char*>(&width), sizeof(width));
//     ofs.write(reinterpret_cast<char*>(&height), sizeof(height));

//     for (size_t y = 0; y < image->height(); y++) {
//         for (size_t x = 0; x < image->width(); x++) {
//             Rgba8Pixel pixel = image->get_rgba8_lossy(x, y);
//             ofs.write(reinterpret_cast<char*>(&pixel), sizeof(pixel));

//             // println("Pixel {}, {} : {} {} {} {}", x, y, pixel.r, pixel.g, pixel.b, pixel.a);
//         }
//     }

//     return 0;
// }