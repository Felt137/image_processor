#ifndef CPP_HSE_COLOR_H
#define CPP_HSE_COLOR_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include "../Overloading/Vector_operations.h"
#include "../Reading_and_writing/Utils.h"

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Color() = default;
    explicit Color(const std::vector<uint8_t> &rgb);
    explicit Color(const std::array<uint8_t, 3> &rgb);
    template <typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>) explicit Color(T brightness) {
        *this = Color(std::vector<uint8_t>(3, static_cast<uint8_t>(brightness)));
    }
    template <typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>) explicit Color(const std::vector<T> &rgb)
        : red(static_cast<uint8_t>(std::clamp(rgb[0], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                              static_cast<T>(image::utils::MAX_COLOR_VALUE)))),
          green(static_cast<uint8_t>(std::clamp(rgb[1], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                                static_cast<T>(image::utils::MAX_COLOR_VALUE)))),
          blue(static_cast<uint8_t>(std::clamp(rgb[2], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                               static_cast<T>(image::utils::MAX_COLOR_VALUE)))) {
    }
    template <typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>) explicit Color(const std::array<T, 3> &rgb)
        : red(static_cast<uint8_t>(std::clamp(rgb[0], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                              static_cast<T>(image::utils::MAX_COLOR_VALUE)))),
          green(static_cast<uint8_t>(std::clamp(rgb[1], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                                static_cast<T>(image::utils::MAX_COLOR_VALUE)))),
          blue(static_cast<uint8_t>(std::clamp(rgb[2], static_cast<T>(image::utils::MIN_COLOR_VALUE),
                                               static_cast<T>(image::utils::MAX_COLOR_VALUE)))) {
    }

    template <typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::vector<T> GetVector()
    const {
        return {static_cast<T>(red), static_cast<T>(green), static_cast<T>(blue)};
    }

    template <typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::array<T, 3> GetArray()
    const {
        return {static_cast<T>(red), static_cast<T>(green), static_cast<T>(blue)};
    }

    static Color White();
    static Color Black();
};

#endif  // CPP_HSE_COLOR_H