#ifndef CPP_HSE_VECTOR_OPERATIONS_H
#define CPP_HSE_VECTOR_OPERATIONS_H

#include <vector>
#include <stdexcept>

template <typename T1, typename T2>
requires((std::is_integral_v<T1> || std::is_floating_point_v<T1>)&&(std::is_integral_v<T2> ||
                                                                    std::is_floating_point_v<T2>)) std::vector<T1>
&operator*=(std::vector<T1>& lhs, const T2 rhs) {
    for (auto& i : lhs) {
        i *= rhs;
    }
    return lhs;
}

template <typename T1, typename T2>
requires((std::is_integral_v<T1> || std::is_floating_point_v<T1>)&&(std::is_integral_v<T2> ||
                                                                    std::is_floating_point_v<T2>)) std::vector<T1>
operator*(const std::vector<T1>& lhs, const T2 rhs) {
    std::vector<T1> result = lhs;
    result *= rhs;
    return result;
}

template <typename T>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::vector<T>
&operator+=(std::vector<T>& lhs, const std::vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::invalid_argument("Error while adding two std::vector objects: vectors must be the same size");
    }
    for (size_t i = 0; i < lhs.size(); ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

template <typename T>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>) std::vector<T>
operator+(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    std::vector<T> result = lhs;
    result += rhs;
    return result;
}

#endif  // CPP_HSE_VECTOR_OPERATIONS_H
