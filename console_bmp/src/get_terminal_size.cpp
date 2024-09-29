#include <console_bmp/get_terminal_size.hpp>

#ifdef WIN32
#include <windows.h>

    namespace console_bmp {

    std::pair<size_t, size_t> get_terminal_size() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        size_t width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size_t height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return std::pair<size_t, size_t>(width, height);
    }

    } // namespace console_bmp
#else

    #include <sys/ioctl.h>
    #include <unistd.h>

    namespace console_bmp {

    std::pair<size_t, size_t> get_terminal_size() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        size_t width = w.ws_col;
        size_t height = w.ws_row;
        return std::pair<size_t, size_t>(width, height);
    }

    } // namespace console_bmp
#endif