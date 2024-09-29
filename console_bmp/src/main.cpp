#include <bmp_reader/util/print.hpp>
#include <console_bmp/cli_args.hpp>
#include <console_bmp/bmp_display.hpp>
#include <console_bmp/get_terminal_size.hpp>


using bmp_reader::eprintln;

using namespace console_bmp;

int main(int argc, char** argv) {
    // Parse args
    AppArguments cli_args; 
    try {
        cli_args = parse_args(argc, argv);
    } catch (const std::exception& e) {
        eprintln("Failed to parse arguments: {}", e.what());
        return 1;
    }

    // Do the rest
    BmpDisplay display(cli_args.gradient, cli_args.info);
    
    display.openBMP(cli_args.bmp_file_path.string());

    if (cli_args.auto_width) {
        auto [w, h] = get_terminal_size();
        cli_args.width = w;
        cli_args.height = h;
    }

    if (!cli_args.no_raw_text)
        display.displayBMP(cli_args.width, cli_args.height);


#ifdef CONSOLE_BMP_SFML_WINDOW
    if (cli_args.out_window)
        display.displayBMPInWindow();
#endif

    display.closeBMP();

    return 0;
}