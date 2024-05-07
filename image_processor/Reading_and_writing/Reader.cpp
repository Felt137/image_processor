#include "Reader.h"

reading_and_writing::Reader::Reader(const std::string& path) : path_(path) {
    fin_.open(path_, std::ios_base::in | std::ios_base::binary);
    if (!fin_.is_open()) {
        throw exceptions::CannotOpenFile(path_, exceptions::CannotOpenFile::reading);
    }
    if (errno == EACCES) {
        throw exceptions::CannotOpenFile(path_, exceptions::CannotOpenFile::reading,
                                         exceptions::CannotOpenFile::access_denied);
    }
    noskipws(fin_);
    fin_.tie(0);
    std::ios_base::sync_with_stdio(false);
}

size_t reading_and_writing::Reader::ReadConsecutiveBytesAsNumber(const size_t number_of_bytes) {
    uint32_t result = 0;
    for (size_t byte_number = 0; byte_number < number_of_bytes; ++byte_number) {
        uint8_t byte = 0;
        fin_ >> byte;
        result += byte << byte_number * image::utils::NUMBER_OF_BITS_IN_BYTE;
    }
    return result;
}

std::vector<uint8_t> reading_and_writing::Reader::ReadConsecutiveBytesAsVector(const size_t number_of_bytes) {
    std::vector<uint8_t> result;
    for (size_t byte_number = 0; byte_number < number_of_bytes; ++byte_number) {
        uint8_t byte = fin_.peek();
        fin_.ignore(0);
        fin_ >> byte;
        result.push_back(byte);
    }
    return result;
}

Image reading_and_writing::Reader::Read() {
    try {
        std::vector<uint8_t> first_54_bytes_unmodified =
            ReadConsecutiveBytesAsVector(image::utils::INFORMATION_HEADER_SIZE + image::utils::FILE_HEADER_SIZE);
        fin_.seekg(0);
        std::vector<uint8_t> signature = ReadConsecutiveBytesAsVector(2);
        if (std::vector<uint8_t>{static_cast<uint8_t>(signature[0]), static_cast<uint8_t>(signature[1])} !=
            image::utils::BMP_SIGNATURE) {
        }
        // size_t file_size = ReadConsecutiveBytesAsNumber(4);
        fin_.ignore(image::utils::FILE_SIZE_SIZE);
        fin_.ignore(image::utils::USELESS_SECTIONS_SIZE[0]);
        size_t image_width = ReadConsecutiveBytesAsNumber(4);
        size_t padding_size = Image::CalculatePaddingSize(image_width);
        size_t image_height = ReadConsecutiveBytesAsNumber(4);
        if (image_width == 0 || image_height == 0) {
            throw std::invalid_argument("Input image is empty.");
        }
        fin_.ignore(image::utils::USELESS_SECTIONS_SIZE[1]);
        //        size_t image_size = ReadConsecutiveBytesAsNumber(4);
        fin_.ignore(image::utils::IMAGE_SIZE_SIZE);
        fin_.ignore(image::utils::USELESS_SECTIONS_SIZE[2]);
        std::vector<std::vector<Color>> color_matrix(image_height, std::vector<Color>(image_width));
        for (size_t i = 0; i < image_height; i++) {
            for (size_t j = 0; j < image_width; j++) {
                std::vector<uint8_t> colors = ReadConsecutiveBytesAsVector(3);
                std::reverse(colors.begin(), colors.end());
                color_matrix[i][j] = Color(colors);
            }
            fin_.ignore(static_cast<int32_t>(padding_size));
        }
        std::reverse(color_matrix.begin(), color_matrix.end());
        //    color_matrix = std::vector<std::vector<Color>>{{Color({128, 128, 128}), Color({128, 128, 128}),
        //    Color({128, 128, 128}), Color({128, 128, 128}), Color({128, 128, 128}), Color({128, 128, 128}),},
        //                                                   {Color({128, 128, 128}), Color({128, 128, 128}),
        //                                                   Color({128, 128, 128}), Color({128, 128, 128}), Color({128,
        //                                                   128, 128}), Color({128, 128, 128}),}, {Color({128, 128,
        //                                                   128}), Color({128, 128, 128}), Color({128, 128, 128}),
        //                                                   Color({128, 128, 128}), Color({128, 128, 128}), Color({128,
        //                                                   128, 128}),}, {Color({128, 128, 128}), Color({128, 128,
        //                                                   128}), Color({128, 128, 128}), Color({128, 128, 128}),
        //                                                   Color({128, 128, 128}), Color({128, 128, 128}),},
        //                                                   {Color({128, 128, 128}), Color({128, 128, 128}),
        //                                                   Color({128, 128, 128}), Color({128, 128, 128}), Color({128,
        //                                                   128, 128}), Color({128, 128, 128}),}, {Color({128, 128,
        //                                                   128}), Color({128, 128, 128}), Color({128, 128, 128}),
        //                                                   Color({128, 128, 128}), Color({128, 128, 128}), Color({128,
        //                                                   128, 128}),}};
        return Image(first_54_bytes_unmodified, color_matrix);
    } catch (std::exception& e) {
        throw exceptions::ErrorWhileReadingFile(path_);
    }
}