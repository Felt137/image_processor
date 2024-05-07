#ifndef CPP_HSE_MAIN_H
#define CPP_HSE_MAIN_H

#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Filters/Filters.h"
#include "Image/Image.h"
#include "Parser/Parser.h"
#include "Perlin_noise/Perlin_noise.h"
#include "Reading_and_writing/Reader.h"
#include "Reading_and_writing/Writer.h"

Image ReadImage(const std::string& path);
Image ApplyFilters(const Image& image, const std::vector<parser::Token>& tokens);
void WriteImage(const std::string& path, const Image& image);

#endif  // CPP_HSE_MAIN_H
