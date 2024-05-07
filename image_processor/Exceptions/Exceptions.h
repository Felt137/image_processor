#ifndef CPP_HSE_EXCEPTIONS_H
#define CPP_HSE_EXCEPTIONS_H

#include <stdexcept>

namespace exceptions {

class WrongFilterArgumentType : public std::exception {
    std::string message_;

public:
    explicit WrongFilterArgumentType(const std::string& filter_name);
    const char* what() const noexcept override;
};

class WrongNumberOfArguments : public std::exception {
    std::string message_;

public:
    WrongNumberOfArguments(const std::string& object, const size_t expected, const size_t provided);
    const char* what() const noexcept override;
};

class WrongTransformMatrixSize : public std::exception {
    std::string message_;

public:
    WrongTransformMatrixSize(const size_t height, const size_t width);
    const char* what() const noexcept override;
};

class WrongFilterName : public std::exception {
    std::string message_;

public:
    explicit WrongFilterName(const std::string& name);
    const char* what() const noexcept override;
};

class CannotOpenFile : public std::exception {
    std::string message_;

public:
    enum mode { reading, writing, mode_not_specified };
    enum error_type { access_denied, error_type_not_specified };

    explicit CannotOpenFile(const std::string& path, mode mode = mode_not_specified,
                            error_type error_type = error_type_not_specified);
    const char* what() const noexcept override;
};

class ErrorWhileReadingFile : public std::exception {
    std::string message_;

public:
    explicit ErrorWhileReadingFile(const std::string& path);
    const char* what() const noexcept override;
};

}  // namespace exceptions

#endif  // CPP_HSE_EXCEPTIONS_H
