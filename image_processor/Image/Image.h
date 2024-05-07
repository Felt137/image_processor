#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>
#include "../Reading_and_writing/Utils.h"
#include "Color.h"

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);
    Image(const std::vector<uint8_t>& first_54_bytes_unmodified, const std::vector<std::vector<Color>>& color_matrix);
    ~Image() = default;

    static size_t CalculatePaddingSize(size_t width);
    static std::vector<std::vector<Color>> BrightnessMatrixToColorMatrix(
        std::vector<std::vector<uint8_t>> brightness_matrix) {
        assert(!brightness_matrix.empty());
        int32_t height = static_cast<int32_t>(brightness_matrix.size());
        int32_t width = static_cast<int32_t>(brightness_matrix.front().size());
        std::vector<std::vector<Color>> color_matrix(height, std::vector<Color>(width));
        for (int32_t i = 0; i < height; ++i) {
            for (int32_t j = 0; j < width; ++j) {
                color_matrix[i][j] = Color(brightness_matrix[i][j]);
            }
        }
        return color_matrix;
    }

    const std::vector<uint8_t>& GetFirst54Bytes() const;
    const std::vector<std::vector<Color>>& GetColorMatrix() const;

    void InitializeImageParameters();
    void InitializeImageSize();
    void InitializeFileSize();
    void InitializeWidthAndHeight();
    void InitializePaddingSize();

    size_t file_size = 0;
    size_t image_size = 0;
    size_t padding_size = 0;
    size_t width_ = 0;
    size_t height_ = 0;

private:
    std::vector<uint8_t> first_54_bytes_unmodified_;
    std::vector<std::vector<Color>> color_matrix_;
};
#endif  // CPP_HSE_IMAGE_H