#ifndef CPP_HSE_WRITER_H
#define CPP_HSE_WRITER_H

#include "../Image/Image.h"
#include "Utils.h"
#include "../Exceptions/Exceptions.h"

namespace reading_and_writing {
class Writer {
public:
    explicit Writer(const std::string& path);
    void Write(const Image& image);
    void WriteConsecutiveBytes(const std::vector<uint8_t>& vector);
    void WriteConsecutiveBytes(const std::vector<uint8_t>::const_iterator begin,
                               const std::vector<uint8_t>::const_iterator end);
    void WriteUselessBytes(const size_t number_of_bytes);
    void WriteNumber(const size_t number, const size_t length_in_bytes);

private:
    std::string path_;
    std::ofstream fout_;
    void WriteUselessSectionOfHeader(const Image& image);
    int32_t useless_section_number_ = 0;
};
}  // namespace reading_and_writing

#endif  // CPP_HSE_WRITER_H
