#ifndef CPP_HSE_ARRAY_OPERATIONS_H
#define CPP_HSE_ARRAY_OPERATIONS_H

#include <array>
#include <stddef.h>

template <typename T1, typename T2, size_t size>
requires((std::is_integral_v<T1> || std::is_floating_point_v<T1>)&&(std::is_integral_v<T2> ||
                                                                    std::is_floating_point_v<T2>)) std::array<T1, size>
&operator*=(std::array<T1, size>& lhs, const T2 rhs) {
    for (auto& i : lhs) {
        i *= rhs;
    }
    return lhs;
}

template <typename T1, typename T2, size_t size>
requires((std::is_integral_v<T1> || std::is_floating_point_v<T1>)&&(std::is_integral_v<T2> ||
                                                                    std::is_floating_point_v<T2>)) std::array<T1, size>
operator*(const std::array<T1, size>& lhs, const T2 rhs) {
    std::array<T1, size> result = lhs;
    result *= rhs;
    return result;
}

template <typename T, size_t size>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::array<T, size>
&operator+=(std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

template <typename T, size_t size>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::array<T, size>
operator+(const std::array<T, size>& lhs, const std::array<T, size>& rhs) {
    std::array<T, size> result = lhs;
    result += rhs;
    return result;
}

#endif  // CPP_HSE_ARRAY_OPERATIONS_H
