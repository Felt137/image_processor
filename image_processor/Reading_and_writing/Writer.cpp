#include "Writer.h"

reading_and_writing::Writer::Writer(const std::string& path) : path_{path} {
    fout_.open(path_, std::ios::out | std::ios::binary);
    if (!fout_.is_open()) {
        throw exceptions::CannotOpenFile(path_, exceptions::CannotOpenFile::writing);
    }
    if (errno == EACCES) {
        throw exceptions::CannotOpenFile(path_, exceptions::CannotOpenFile::writing,
                                         exceptions::CannotOpenFile::access_denied);
    }
    fout_.tie(0);
    std::ios_base::sync_with_stdio(false);
}

void reading_and_writing::Writer::WriteConsecutiveBytes(const std::vector<uint8_t>& vector) {
    WriteConsecutiveBytes(vector.begin(), vector.end());
}

void reading_and_writing::Writer::WriteConsecutiveBytes(const std::vector<uint8_t>::const_iterator begin,
                                                        const std::vector<uint8_t>::const_iterator end) {
    for (auto byte = begin; byte < end; ++byte) {
        fout_ << *byte;
    }
}

void reading_and_writing::Writer::WriteUselessBytes(const size_t number_of_bytes) {
    for (size_t i = 0; i < number_of_bytes; ++i) {
        fout_ << static_cast<char>(0);
    }
}

void reading_and_writing::Writer::WriteNumber(const size_t number, const size_t length_in_bytes) {
    for (size_t byte_number = 0; byte_number < length_in_bytes; ++byte_number) {
        uint8_t byte = 0;
        byte |= number >> (byte_number * image::utils::NUMBER_OF_BITS_IN_BYTE);
        fout_ << byte;
    }
}

void reading_and_writing::Writer::Write(const Image& image) {
    WriteConsecutiveBytes(image::utils::BMP_SIGNATURE);
    WriteNumber(image.file_size, image::utils::FILE_SIZE_SIZE);
    WriteUselessSectionOfHeader(image);
    WriteNumber(image.width_, image::utils::WIDTH_SIZE);
    WriteNumber(image.height_, image::utils::HEIGHT_SIZE);
    WriteUselessSectionOfHeader(image);
    WriteNumber(image.image_size, image::utils::IMAGE_SIZE_SIZE);
    WriteUselessSectionOfHeader(image);
    const std::vector<std::vector<Color>>& color_matrix = image.GetColorMatrix();
    for (auto row = color_matrix.rbegin(); row != color_matrix.rend(); ++row) {
        for (auto& pixel : *row) {
            WriteConsecutiveBytes({pixel.blue, pixel.green, pixel.red});
        }
        WriteUselessBytes(image.padding_size);
    }
}

void reading_and_writing::Writer::WriteUselessSectionOfHeader(const Image& image) {
    WriteConsecutiveBytes(
        image.GetFirst54Bytes().begin() + image::utils::USELESS_SECTIONS_OFFSET[useless_section_number_][0],
        image.GetFirst54Bytes().begin() + image::utils::USELESS_SECTIONS_OFFSET[useless_section_number_][1]);
    ++useless_section_number_;
}
