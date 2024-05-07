#include "Exceptions.h"

namespace exceptions {

WrongFilterArgumentType::WrongFilterArgumentType(const std::string& filter_name)
    : message_("Wrong argument type for \"" + filter_name + "\" filter.") {
}

const char* WrongFilterArgumentType::what() const noexcept {
    return message_.c_str();
}

WrongNumberOfArguments::WrongNumberOfArguments(const std::string& object, const size_t expected, const size_t provided)
    : message_("Too " + (expected > provided ? std::string("few") : std::string("many")) + " arguments for \"" +
               object + "\": " + std::to_string(expected) + " expected, " + std::to_string(provided) + " provided.") {
}

const char* WrongNumberOfArguments::what() const noexcept {
    return message_.c_str();
}

WrongTransformMatrixSize::WrongTransformMatrixSize(const size_t height, const size_t width)
    : message_("Wrong matrix size for one of the filters: expected odd dimensions, got " + std::to_string(height) +
               " by " + std::to_string(width) + " instead.") {
}

const char* WrongTransformMatrixSize::what() const noexcept {
    return message_.c_str();
}

WrongFilterName::WrongFilterName(const std::string& name)
    : message_("Wrong filter name: \"" + name + "\". Available filters are: -crop, -gs, -neg, -sharp, -edge, -blur.") {
}

const char* WrongFilterName::what() const noexcept {
    return message_.c_str();
}

CannotOpenFile::CannotOpenFile(const std::string& path, exceptions::CannotOpenFile::mode mode,
                               exceptions::CannotOpenFile::error_type error_type) {
    message_ = "Cannot open file " + path;
    if (mode == reading) {
        message_ += " for reading";
    } else if (mode == writing) {
        message_ += " for writing";
    }
    if (error_type == access_denied) {
        message_ += ": access denied";
    }
    message_ += '.';
}

const char* CannotOpenFile::what() const noexcept {
    return message_.c_str();
}

ErrorWhileReadingFile::ErrorWhileReadingFile(const std::string& path)
    : message_("Error while reading file " + path +
               ": file is corrupted or utilizes currently unsupported BMP format.") {
}

const char* ErrorWhileReadingFile::what() const noexcept {
    return message_.c_str();
}

};  // namespace exceptions
