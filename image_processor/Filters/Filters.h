#ifndef CPP_HSE_FILTERS_H
#define CPP_HSE_FILTERS_H

#include "../Overloading/Array_operations.h"
#include <cctype>
#include <cmath>
#include <iomanip>
#include <memory>
#include "../Exceptions/Exceptions.h"
#include "../Image/Image.h"
#include "../Overloading/Vector_operations.h"
#include "../Parser/Parser.h"
#include "../Perlin_noise/Perlin_noise.h"

namespace filters {
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    Image Apply(const Image& image);
    virtual std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) = 0;
};

template <typename T>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>) Color
    MultiplyPixelByMatrix(const std::vector<std::vector<Color>>& color_matrix,
                          const std::vector<std::vector<T>>& transform_matrix, int32_t x, int32_t y) {
    std::array<T, 3> result = color_matrix[x][y].GetArray<T>() * transform_matrix[1][1];
    const std::vector<std::pair<int32_t, int32_t>> offsets = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto [i, j] : offsets) {
        result += color_matrix[std::clamp(x + j, 0, static_cast<int32_t>(color_matrix.size() - 1))]
                              [std::clamp(y + i, 0, static_cast<int32_t>(color_matrix.front().size() - 1))]
                                  .GetArray<T>() *
                  transform_matrix[1 + i][1 + j];
    }
    return Color(result);
}

template <typename T>
requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    std::vector<std::vector<Color>> MultiplyEveryPixelByMatrix(const std::vector<std::vector<Color>>& color_matrix,
                                                               const std::vector<std::vector<T>>& transform_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    for (size_t i = 0; i < color_matrix.size(); ++i) {
        for (size_t j = 0; j < color_matrix.front().size(); ++j) {
            new_color_matrix[i][j] =
                MultiplyPixelByMatrix(color_matrix, transform_matrix, static_cast<int32_t>(i), static_cast<int32_t>(j));
        }
    }
    return new_color_matrix;
}

std::unique_ptr<Filter> GetFilter(const parser::Token& token);

void CheckNumberOfArguments(std::string object, const size_t expected, const size_t provided);

class Crop : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 2;
    Crop(const size_t width, const size_t height);
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;

private:
    size_t new_width_;
    size_t new_height_;
};

class Grayscale : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 0;
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;
};

class Negative : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 0;
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;
};

class Sharpening : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 0;
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;

private:
    const std::vector<std::vector<int32_t>> transform_matrix_ = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
};

class EdgeDetection : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 1;
    explicit EdgeDetection(double threshold);
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;

private:
    double threshold_;
    const std::vector<std::vector<int32_t>> transform_matrix_ = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
};

class GaussianBlur : public Filter {
public:
    constexpr static const size_t ExpectedNumberOfArguments = 1;
    explicit GaussianBlur(double sigma);
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;

private:
    const int32_t radius_ = 13;
    double sigma_;
    std::vector<double> horizontal_transform_matrix_;
    std::vector<double> vertical_transform_matrix_;
    std::vector<std::vector<double>> gaussian_function_matrix_prefix_sum_;
    double CalculateGaussianFunction(int32_t horizontal_distance, int32_t vertical_distance) const;
    void InitializeGaussianFunctionMatrixPrefixSum();
    double GetNormCoeffForPixel(int32_t i, int32_t j, int32_t height, int32_t width) const;
};

class Distortion : public Filter {
public:
    //    Distortion(double scale = DefaultScale, double strength = DefaultStrength);
    Distortion(double scale, double strength);
    constexpr static const size_t ExpectedNumberOfArguments = 2;
    std::vector<std::vector<Color>> Apply(const std::vector<std::vector<Color>>& color_matrix) override;

private:
    constexpr static double MaxStrength = 100.0;
    double scale_;
    double strength_;
};

}  // namespace filters

#endif  // CPP_HSE_FILTERS_H
