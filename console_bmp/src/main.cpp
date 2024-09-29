#include <bmp_reader/util/print.hpp>
#include <console_bmp/cli_args.hpp>
#include <console_bmp/bmp_display.hpp>


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

    BmpDisplay display;
    
    display.setShowInfo(cli_args.info);
    display.openBMP(cli_args.bmp_file_path.string());

    if (!cli_args.no_raw_text)
        display.displayBMP(cli_args.width, cli_args.height);

    if (cli_args.out_window)
        display.displayBMPInWindow();

    display.closeBMP();

    return 0;
}