#include <console_bmp/bmp_display.hpp>

#include <thread>

#include <bmp_reader/util/print.hpp>

#ifndef CONSOLE_BMP_SFML_WINDOW
    using bmp_reader::eprintln;
#else
    #include <SFML/Graphics.hpp>
#endif

namespace console_bmp {

void BmpDisplay::displayBMPInWindow() {
    #ifndef CONSOLE_BMP_SFML_WINDOW
        eprintln("Displaying BMP in window is not supported");
    #else
        sf::Texture texture;
        texture.create(m_image->width(), m_image->height());
        texture.update(reinterpret_cast<uint8_t*>(m_image->data()));

        // Create a sprite to display the texture
        sf::Sprite sprite(texture);

        sprite.setScale(
            m_image->flipped_w() ? -1.0f : 1.0f, 
            m_image->flipped_h() ?  -1.0f : 1.0f
        );

        // Create a window
        sf::RenderWindow window(sf::VideoMode(m_image->width(), m_image->height()), "RGBA8 Image");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(sprite);
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    #endif
}


} // namespace console_bmp