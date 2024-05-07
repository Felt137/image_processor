#include "Image.h"

Image::Image(size_t width, size_t height) : width_{width}, height_{height} {
}

Image::Image(const std::vector<uint8_t>& first_54_bytes_unmodified, const std::vector<std::vector<Color>>& color_matrix)
    : first_54_bytes_unmodified_(first_54_bytes_unmodified), color_matrix_(color_matrix) {
    InitializeImageParameters();
}

void Image::InitializeImageParameters() {
    InitializeWidthAndHeight();
    InitializeImageSize();
    InitializeFileSize();
    InitializePaddingSize();
}

size_t Image::CalculatePaddingSize(size_t width) {
    return (4 - width * 3 % 4) % 4;
}

void Image::InitializeWidthAndHeight() {
    width_ = color_matrix_.front().size();
    height_ = color_matrix_.size();
}

void Image::InitializeImageSize() {
    image_size = width_ * height_ * image::utils::BYTES_PER_PIXEL;
}

void Image::InitializeFileSize() {
    file_size = image_size + image::utils::FILE_HEADER_SIZE + image::utils::INFORMATION_HEADER_SIZE;
}

void Image::InitializePaddingSize() {
    padding_size = CalculatePaddingSize(width_);
}

const std::vector<uint8_t>& Image::GetFirst54Bytes() const {
    return first_54_bytes_unmodified_;
}

const std::vector<std::vector<Color>>& Image::GetColorMatrix() const {
    return color_matrix_;
}
