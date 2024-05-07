#include "Color.h"

Color::Color(const std::vector<uint8_t> &rgb) : red(rgb[0]), green(rgb[1]), blue(rgb[2]) {
}

Color Color::White() {
    return Color(std::vector<uint8_t>{image::utils::MAX_COLOR_VALUE, image::utils::MAX_COLOR_VALUE,
                                      image::utils::MAX_COLOR_VALUE});
}

Color Color::Black() {
    return Color(std::vector<uint8_t>{image::utils::MIN_COLOR_VALUE, image::utils::MIN_COLOR_VALUE,
                                      image::utils::MIN_COLOR_VALUE});
}
