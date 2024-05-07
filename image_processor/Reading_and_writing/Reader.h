#ifndef CPP_HSE_READER_H
#define CPP_HSE_READER_H

#include "../Exceptions/Exceptions.h"
#include "../Image/Image.h"
#include "Utils.h"

namespace reading_and_writing {
class Reader {
public:
    explicit Reader(const std::string& path);
    Image Read();

private:
    std::string path_;
    size_t ReadConsecutiveBytesAsNumber(const size_t number_of_bytes);
    std::vector<uint8_t> ReadConsecutiveBytesAsVector(const size_t number_of_bytes);
    std::ifstream fin_;
};
}  // namespace reading_and_writing

#endif  // CPP_HSE_READER_H
