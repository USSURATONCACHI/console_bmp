#include <bmp_reader/dib_headers/os22x_parser.hpp>

namespace bmp_reader {
namespace dib_headers {

// Parser
auto OS22X_Parser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    return header_size == OS22X_Parser::HEADER_IN_FILE_SIZE;
}

static void check_errors(OS22X header);

auto OS22X_Parser::parse_os22x(std::istream& is) -> OS22X {
    WinInfo base_header = parse_win_info(is);

    OS22X header(base_header);

    is.ignore(2); // units for the horizontal and vertical resolutions
    is.ignore(2); // padding
    is.ignore(2); // direction in which the bits fill the bitmap
    is.read(reinterpret_cast<char*>(&header.halftoning_alg), sizeof(header.halftoning_alg));
    is.read(reinterpret_cast<char*>(&header.halftoning_param_1), sizeof(header.halftoning_param_1));
    is.read(reinterpret_cast<char*>(&header.halftoning_param_2), sizeof(header.halftoning_param_2));
    is.ignore(4); // color encoding for each entry in the color table
    is.ignore(4); // An application-defined identifier

    check_errors(header);

    return header;
}

static void check_errors(OS22X header) {
    switch (header.halftoning_alg) {
        case HalftoningAlg::NONE:
        case HalftoningAlg::ERROR_DIFFUSION:
        case HalftoningAlg::PANDA:
        case HalftoningAlg::SUPER_CIRCLE:
            break;

        default:
            throw InvalidHeaderException("invalid halftoning algorithm specified");
    }
}

} // namespace dib_headers
} // namespace bmp_reader