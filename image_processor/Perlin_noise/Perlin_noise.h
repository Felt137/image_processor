#ifndef CPP_HSE_PERLIN_NOISE_H
#define CPP_HSE_PERLIN_NOISE_H

#include <cstdint>
#include <stdexcept>
#include <vector>
#include "../Image/Color.h"
#include "../Image/Image.h"
#include "../Misc/Random.h"
#include "../Vector/Vector2D.h"

class PerlinNoise {
public:
    PerlinNoise() = default;
    std::vector<std::vector<double>> Get(int32_t height, int32_t width,
                                         double scale = DefaultStartingGridSizeToImageRatio);

private:
    std::vector<std::vector<double>> GenerateOctave(int32_t height, int32_t width, int32_t grid_size);
    void MergeOctaves(std::vector<std::vector<double>> &all_previous, std::vector<std::vector<double>> current_octave,
                      double merge_weight);
    double GetPixelBrightness(int32_t x, int32_t y, std::vector<std::vector<Vector2D>> &grid_vectors,
                              int32_t grid_size);
    double InterpolateCubic(double from, double to, double weight);
    constexpr static int32_t NumberOfOctaves = 5;
    constexpr static double DefaultStartingGridSizeToImageRatio = 0.1;
    constexpr static double ContrastMultiplier = 1.2;
    constexpr static double MinClampValue = -1.0;
    constexpr static double MaxClampValue = 1.0;
    constexpr static double RangeOfValues = MaxClampValue - MinClampValue;
};

#endif  // CPP_HSE_PERLIN_NOISE_H
