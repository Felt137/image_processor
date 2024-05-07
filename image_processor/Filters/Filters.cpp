#include "Filters.h"

Image filters::Filter::Apply(const Image& image) {
    return Image(image.GetFirst54Bytes(), Apply(image.GetColorMatrix()));
}

std::unique_ptr<filters::Filter> filters::GetFilter(const parser::Token& token) {
    const std::string& name = token.name;
    std::unique_ptr<filters::Filter> filter = nullptr;
    if (name == "-crop") {
        CheckNumberOfArguments(name, filters::Crop::ExpectedNumberOfArguments, token.args.size());
        size_t width = 0;
        size_t height = 0;
        try {
            width = std::stoi(token.args[0]);
            height = std::stoi(token.args[1]);
        } catch (const std::invalid_argument& e) {
            throw exceptions::WrongFilterArgumentType(name);
        }
        filter = std::make_unique<filters::Crop>(width, height);
    } else if (name == "-gs") {
        CheckNumberOfArguments(name, filters::Grayscale::ExpectedNumberOfArguments, token.args.size());
        filter = std::make_unique<filters::Grayscale>();
    } else if (name == "-neg") {
        CheckNumberOfArguments(name, filters::Negative::ExpectedNumberOfArguments, token.args.size());
        filter = std::make_unique<filters::Negative>();
    } else if (name == "-sharp") {
        CheckNumberOfArguments(name, filters::Sharpening::ExpectedNumberOfArguments, token.args.size());
        filter = std::make_unique<filters::Sharpening>();
    } else if (name == "-edge") {
        CheckNumberOfArguments(name, filters::EdgeDetection::ExpectedNumberOfArguments, token.args.size());
        double threshold = 0.0;
        try {
            threshold = std::stod(token.args[0]);
        } catch (const std::invalid_argument& e) {
            throw exceptions::WrongFilterArgumentType(name);
        }
        filter = std::make_unique<filters::EdgeDetection>(threshold);
    } else if (name == "-blur") {
        CheckNumberOfArguments(name, filters::GaussianBlur::ExpectedNumberOfArguments, token.args.size());
        double sigma = 0.0;
        try {
            sigma = std::stod(token.args[0]);
        } catch (const std::invalid_argument& e) {
            throw exceptions::WrongFilterArgumentType(name);
        }
        filter = std::make_unique<filters::GaussianBlur>(sigma);
    } else if (name == "-my") {
        CheckNumberOfArguments(name, filters::Distortion::ExpectedNumberOfArguments, token.args.size());
        double scale = 0.0;
        double strength = 0.0;
        try {
            scale = std::stod(token.args[0]);
            strength = std::stod(token.args[1]);
        } catch (const std::invalid_argument& e) {
            throw exceptions::WrongFilterArgumentType(name);
        }
        filter = std::make_unique<filters::Distortion>(scale, strength);
    } else {
        throw exceptions::WrongFilterName(token.name);
    }
    return filter;
}

void filters::CheckNumberOfArguments(std::string filter_name, const size_t expected, const size_t provided) {
    if (expected != provided) {
        throw exceptions::WrongNumberOfArguments(filter_name, expected, provided);
    }
}

filters::Crop::Crop(const size_t width, const size_t height) : new_width_(width), new_height_(height) {
    if (new_width_ == 0 || new_height_ == 0) {
        throw std::invalid_argument("Both arguments for -crop must be greater than zero.");
    }
}

std::vector<std::vector<Color>> filters::Crop::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    new_height_ = std::min(new_height_, new_color_matrix.size());
    new_width_ = std::min(new_width_, new_color_matrix.front().size());
    new_color_matrix.resize(new_height_);
    for (auto& row : new_color_matrix) {
        row.resize(new_width_);
    }
    return new_color_matrix;
}

std::vector<std::vector<Color>> filters::Grayscale::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    const double red_multiplier = 0.299;
    const double green_multiplier = 0.587;
    const double blue_multiplier = 0.114;
    for (auto& row : new_color_matrix) {
        for (auto& pixel : row) {
            double merged_color = static_cast<double>(pixel.red) * red_multiplier +
                                  static_cast<double>(pixel.green) * green_multiplier +
                                  static_cast<double>(pixel.blue) * blue_multiplier;
            pixel.red = static_cast<uint8_t>(merged_color);
            pixel.green = static_cast<uint8_t>(merged_color);
            pixel.blue = static_cast<uint8_t>(merged_color);
        }
    }
    return new_color_matrix;
}

std::vector<std::vector<Color>> filters::Negative::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    for (size_t i = 0; i < new_color_matrix.size(); i++) {
        for (size_t j = 0; j < new_color_matrix.front().size(); j++) {
            new_color_matrix[i][j].red = image::utils::MAX_COLOR_VALUE - new_color_matrix[i][j].red;
            new_color_matrix[i][j].green = image::utils::MAX_COLOR_VALUE - new_color_matrix[i][j].green;
            new_color_matrix[i][j].blue = image::utils::MAX_COLOR_VALUE - new_color_matrix[i][j].blue;
        }
    }
    return new_color_matrix;
}

std::vector<std::vector<Color>> filters::Sharpening::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    return MultiplyEveryPixelByMatrix(color_matrix, transform_matrix_);
}

filters::EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold) {
}

std::vector<std::vector<Color>> filters::EdgeDetection::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    new_color_matrix = std::make_unique<Grayscale>()->Apply(color_matrix);
    new_color_matrix = MultiplyEveryPixelByMatrix(new_color_matrix, transform_matrix_);
    for (auto& row : new_color_matrix) {
        for (auto& color : row) {
            if (static_cast<double>(color.red) / static_cast<double>(image::utils::MAX_COLOR_VALUE) > threshold_) {
                color = Color::White();
            } else {
                color = Color::Black();
            }
        }
    }
    return new_color_matrix;
}

filters::GaussianBlur::GaussianBlur(double sigma) : sigma_(sigma) {
    horizontal_transform_matrix_.resize(2 * radius_ + 1);
    vertical_transform_matrix_.resize(2 * radius_ + 1);
    for (int32_t vertical_distance = -radius_; vertical_distance <= radius_; ++vertical_distance) {
        vertical_transform_matrix_[radius_ + vertical_distance] = CalculateGaussianFunction(0, vertical_distance);
    }
    for (int32_t horizontal_distance = -radius_; horizontal_distance <= radius_; ++horizontal_distance) {
        horizontal_transform_matrix_[radius_ + horizontal_distance] =
            CalculateGaussianFunction(horizontal_distance, 0) / vertical_transform_matrix_[radius_];
    }
    InitializeGaussianFunctionMatrixPrefixSum();
}

std::vector<std::vector<Color>> filters::GaussianBlur::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    int32_t height = static_cast<int32_t>(color_matrix.size());
    int32_t width = static_cast<int32_t>(color_matrix.front().size());
    std::vector<std::vector<std::array<double, 3>>> weighed_horizontal_color_sum(
        height, std::vector<std::array<double, 3>>(width, std::array<double, 3>{0, 0, 0}));
    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            int32_t min_offset = -std::min(radius_, j);
            int32_t max_offset = std::min(radius_, width - j - 1);
            for (int32_t k = min_offset; k <= max_offset; k++) {
                weighed_horizontal_color_sum[i][j] +=
                    color_matrix[i][j + k].GetArray<double>() * horizontal_transform_matrix_[radius_ + k];
            }
        }
    }
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            std::array<double, 3> weighed_final_color_sum{0, 0, 0};
            int32_t min_offset = -std::min(radius_, i);
            int32_t max_offset = std::min(radius_, height - i - 1);
            for (int32_t k = min_offset; k <= max_offset; k++) {
                weighed_final_color_sum +=
                    weighed_horizontal_color_sum[i + k][j] * vertical_transform_matrix_[radius_ + k];
            }
            weighed_final_color_sum *= GetNormCoeffForPixel(i, j, height, width);
            new_color_matrix[i][j] = Color(weighed_final_color_sum);
        }
    }
    return new_color_matrix;
}

double filters::GaussianBlur::CalculateGaussianFunction(int32_t horizontal_distance, int32_t vertical_distance) const {
    return 1.0 / M_PI / sigma_ / sigma_ / 2 *
           std::exp(-(
               static_cast<double>(horizontal_distance * horizontal_distance + vertical_distance * vertical_distance) /
               2 / sigma_ / sigma_));
}

void filters::GaussianBlur::InitializeGaussianFunctionMatrixPrefixSum() {
    gaussian_function_matrix_prefix_sum_.resize(2 * radius_ + 2, std::vector<double>(2 * radius_ + 2));
    for (int32_t i = 0; i < 2 * radius_ + 1; ++i) {
        for (int32_t j = 0; j < 2 * radius_ + 1; ++j) {
            gaussian_function_matrix_prefix_sum_[i + 1][j + 1] =
                CalculateGaussianFunction(i - radius_, j - radius_) + gaussian_function_matrix_prefix_sum_[i][j + 1] +
                gaussian_function_matrix_prefix_sum_[i + 1][j] - gaussian_function_matrix_prefix_sum_[i][j];
        }
    }
}

double filters::GaussianBlur::GetNormCoeffForPixel(int32_t i, int32_t j, int32_t height, int32_t width) const {
    int32_t min_vertical_index = -std::min(radius_, i) + radius_;
    int32_t max_vertical_index = std::min(radius_, height - i - 1) + radius_;
    int32_t min_horizontal_index = -std::min(radius_, j) + radius_;
    int32_t max_horizontal_index = std::min(radius_, width - j - 1) + radius_;
    if (min_vertical_index > max_vertical_index) {
        std::swap(min_vertical_index, max_vertical_index);
    }
    if (min_horizontal_index > max_horizontal_index) {
        std::swap(min_horizontal_index, max_horizontal_index);
    }
    double partial_matrix_sum = gaussian_function_matrix_prefix_sum_[max_vertical_index + 1][max_horizontal_index + 1] -
                                gaussian_function_matrix_prefix_sum_[min_vertical_index][max_horizontal_index + 1] -
                                gaussian_function_matrix_prefix_sum_[max_vertical_index + 1][min_horizontal_index] +
                                gaussian_function_matrix_prefix_sum_[min_vertical_index][min_horizontal_index];
    return 1.0 / partial_matrix_sum;
}

filters::Distortion::Distortion(double scale, double strength) {
    if (scale <= 0.0 || scale > 1.0) {
        throw std::invalid_argument("Distortion scale must be non-zero and be in range (0.0, 1.0).");
    }
    if (strength <= 0.0 || strength > 100.0) {
        throw std::invalid_argument("Distortion strength must be non-zero and be in range (0.0, " +
                                    std::to_string(MaxStrength) + ").");
    }
    scale_ = scale;
    strength_ = strength;
}

std::vector<std::vector<Color>> filters::Distortion::Apply(const std::vector<std::vector<Color>>& color_matrix) {
    std::vector<std::vector<Color>> new_color_matrix = color_matrix;
    int32_t height = static_cast<int32_t>(color_matrix.size());
    int32_t width = static_cast<int32_t>(color_matrix.front().size());
    const double max_offset = (strength_ / MaxStrength) * std::sqrt(height * width);
    std::vector<std::vector<double>> noise = std::make_unique<PerlinNoise>()->Get(height, width, scale_);
    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            double position_offset_multiplier = std::min(static_cast<double>(j) / max_offset, 1.0);
            int32_t offset = static_cast<int32_t>(noise[i][j] * max_offset * position_offset_multiplier);
            new_color_matrix[i][j] = color_matrix[i][std::max(0, j - offset)];
        }
    }
    return new_color_matrix;
}