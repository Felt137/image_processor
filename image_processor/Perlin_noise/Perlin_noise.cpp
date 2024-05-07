#include "Perlin_noise.h"

std::vector<std::vector<double>> PerlinNoise::Get(int height, int width, double scale) {
    if (height == 0 || width == 0) {
        throw std::invalid_argument("Perlin noise plane must have non-zero dimensions.");
    }
    if (scale <= 0.0 || scale > 1.0) {
        throw std::invalid_argument("Perlin noise scale must be in range (0.0; 1.0).");
    }
    std::vector<std::vector<double>> matrix(height, std::vector<double>(width));
    int grid_size = 1;
    while (static_cast<double>(grid_size) / std::sqrt(height * width) < scale) {
        grid_size *= 2;
    }
    double current_octave_weight = 1;
    for (int current_octave = 0; current_octave < NumberOfOctaves && grid_size >= 1; ++current_octave) {
        MergeOctaves(matrix, GenerateOctave(height, width, grid_size), current_octave_weight);
        current_octave_weight /= 2;
        grid_size /= 2;
    }
    for (auto& row : matrix) {
        for (auto& pixel : row) {
            pixel *= ContrastMultiplier;
            pixel = (std::clamp(pixel, MinClampValue, MaxClampValue) + std::abs(MinClampValue)) / RangeOfValues;
        }
    }
    return matrix;
}

std::vector<std::vector<double>> PerlinNoise::GenerateOctave(int32_t height, int32_t width, int32_t grid_size) {
    int32_t grid_height = (height + grid_size - 1) / grid_size + 1;
    int32_t grid_width = (width + grid_size - 1) / grid_size + 1;
    std::vector<std::vector<Vector2D>> grid_vectors(grid_height, std::vector<Vector2D>(grid_width));
    for (int32_t i = 0; i < grid_height; ++i) {
        for (int32_t j = 0; j < grid_width; ++j) {
            grid_vectors[i][j] = Vector2D::GetRandomVector2D();
        }
    }
    std::vector<std::vector<double>> matrix(height, std::vector<double>(width));
    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            matrix[i][j] = GetPixelBrightness(i, j, grid_vectors, grid_size);
        }
    }
    return matrix;
}

void PerlinNoise::MergeOctaves(std::vector<std::vector<double>>& all_previous,
                               std::vector<std::vector<double>> current_octave, double merge_weight) {
    int32_t height = static_cast<int32_t>(all_previous.size());
    int32_t width = static_cast<int32_t>(all_previous.front().size());
    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            all_previous[i][j] += current_octave[i][j] * merge_weight;
        }
    }
}

double PerlinNoise::GetPixelBrightness(int32_t x, int32_t y, std::vector<std::vector<Vector2D>>& grid_vectors,
                                       int32_t grid_size) {
    Vector2D pixel_vector(x, y);
    int32_t upper_left_grid_index_x = x / grid_size;
    int32_t upper_left_grid_index_y = y / grid_size;
    std::vector<std::vector<double>> dot_products(2, std::vector<double>(2));
    for (int32_t i = 0; i < 2; i++) {
        for (int32_t j = 0; j < 2; j++) {
            int32_t grid_index_x = upper_left_grid_index_x + i;
            int32_t grid_index_y = upper_left_grid_index_y + j;
            Vector2D grid_corner(grid_index_x * grid_size, grid_index_y * grid_size);
            Vector2D grid_to_pixel = pixel_vector - grid_corner;
            //            Vector2D grid_to_pixel = grid_corner - pixel_vector;
            dot_products[i][j] = DotProduct(grid_to_pixel / grid_size, grid_vectors[grid_index_x][grid_index_y]);
        }
    }
    double vertical_weight = static_cast<double>(x - upper_left_grid_index_x * grid_size) / grid_size;
    double horizontal_weight = static_cast<double>(y - upper_left_grid_index_y * grid_size) / grid_size;
    double top_interpolated = InterpolateCubic(dot_products[0][0], dot_products[0][1], horizontal_weight);
    double bottom_interpolated = InterpolateCubic(dot_products[1][0], dot_products[1][1], horizontal_weight);
    double result = InterpolateCubic(top_interpolated, bottom_interpolated, vertical_weight);
    return result;
}

double PerlinNoise::InterpolateCubic(double from, double to, double weight) {
    constexpr static double Coeff1 = 3.0;
    constexpr static double Coeff2 = 2.0;
    return (to - from) * (Coeff1 - weight * Coeff2) * weight * weight + from;
}
