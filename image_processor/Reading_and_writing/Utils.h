#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <algorithm>
#include <bitset>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace image::utils {
const int32_t FILE_HEADER_SIZE = 14;
const int32_t INFORMATION_HEADER_SIZE = 40;
const int32_t SIGNATURE_SIZE = 2;
const int32_t FILE_SIZE_SIZE = 4;
const int32_t IMAGE_SIZE_SIZE = 4;
const int32_t WIDTH_SIZE = 4;
const int32_t HEIGHT_SIZE = 4;
const int32_t PADDING_BYTES = 4;
const int32_t BYTES_PER_PIXEL = 3;
const int32_t HEADER_WIDTH_OFFSET = 4;
const int32_t HEADER_HEIGHT_OFFSET = 4;
const int32_t NUMBER_OF_BITS_IN_BYTE = 8;
const uint8_t MIN_COLOR_VALUE = 0;
const uint8_t MAX_COLOR_VALUE = 255;

const std::vector<uint8_t> BMP_SIGNATURE = {'B', 'M'};

const std::vector<std::vector<int32_t>> USELESS_SECTIONS_OFFSET = {{6, 18}, {26, 34}, {38, 54}};
const std::vector<int32_t> USELESS_SECTIONS_SIZE = {12, 8, 16};
}  // namespace image::utils

#endif  // CPP_HSE_UTILS_H
