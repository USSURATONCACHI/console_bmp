#include <console_bmp/cli_args.hpp>

#include <argparse/argparse.hpp>
#include <filesystem>
#include <stdexcept>

namespace console_bmp {


static void fill_parser(argparse::ArgumentParser& program);
static void add_flag(argparse::ArgumentParser& program, const char* name, const char* short_name, const char* description);
static void add_flag(argparse::ArgumentParser& program, const char* name, const char* description);

auto parse_args(int argc, char** argv) -> AppArguments {
    argparse::ArgumentParser parser(argv[0], "1.1.0", argparse::default_arguments::help, false);
    fill_parser(parser);

    parser.parse_args(argc, argv);

    auto filepath_string = parser.get<std::string>("bmp_file_path");
    int width = parser.get<int>("--width");
    int height = parser.get<int>("--height");

    if (width < 0)
        throw std::runtime_error("Width cannot be negative");

    if (height < 0)
        throw std::runtime_error("Height cannot be negative");

    return AppArguments {
        .info = parser.get<bool>("--info"),

        .gradient     = parser.get<bool>("--gradient"),
        .no_raw_text  = parser.get<bool>("--no-raw-text"),
        .no_auto_size = parser.get<bool>("--no-auto-size"),
#ifdef CONSOLE_BMP_SFML_WINDOW
        .out_window   = parser.get<bool>("--window"),
#endif

        .width  = static_cast<size_t>(width),
        .height = static_cast<size_t>(height),

        .bmp_file_path = std::filesystem::path(filepath_string),
    };
}


static void fill_parser(argparse::ArgumentParser& program) {
    program.add_argument("bmp_file_path")
        .help("show the selected BMP image");

    add_flag(program, "--info", "-i","Show information about image");
    add_flag(program, "--gradient", "-g","Use more than two symbols to represent brightness");
    add_flag(program, "--no-raw-text", "Do not print image to stdout");
    add_flag(program, "--no-auto-size", "-a",  "Do not restrict image by terminal size");
#ifdef CONSOLE_BMP_SFML_WINDOW
    add_flag(program, "--window",      "Open a window to display an image there");
#endif
    program.add_argument("--width", "-w")
        .scan<'i', int>()
        .default_value(0)
        .help("How many characters wide to print to stdout");

    program.add_argument("--height", "-h")
        .scan<'i', int>()
        .default_value(0)
        .help("How many characters tall to print to stdout");
}

static void add_flag(
    argparse::ArgumentParser& program, 
    const char* name,
    const char* short_name, 
    const char* description
) {
    program.add_argument(name, short_name)
        .flag()
        .help(description)
        .default_value(false)
        .implicit_value(true);
}

static void add_flag(
    argparse::ArgumentParser& program, 
    const char* name,
    const char* description
) {
    program.add_argument(name)
        .flag()
        .help(description)
        .default_value(false)
        .implicit_value(true);
}

} // namespace console_bmp