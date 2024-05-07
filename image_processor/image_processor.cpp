#include "image_processor.h"
#include <ctime>

Image ReadImage(const std::string& path) {
    reading_and_writing::Reader reader(path);
    Image image = reader.Read();
    return image;
}

void WriteImage(const std::string& path, const Image& image) {
    reading_and_writing::Writer writer(path);
    writer.Write(image);
}

Image ApplyFilters(const Image& image, const std::vector<parser::Token>& tokens) {
    Image new_image = image;
    for (size_t i = 3; i < tokens.size(); ++i) {
        new_image = filters::GetFilter(tokens[i])->Apply(new_image);
    }
    return new_image;
}

int main(int argc, char** argv) {
    auto st = std::time(nullptr);
    std::vector<parser::Token> tokens = parser::Parse(argc, argv);
    if (tokens.size() == 1) {
        std::cout << "Image processor console application.\n-----\nInput format: \" input_path output_path "
                     "filters_separated_by_space\". Example:"
                     "\n/home/user/image.bmp /home/user/output_image.bmp -crop 10 10 -blur 7.5 -dist\nSee each filter "
                     "input format below.\n-----\n"
                     "-crop width height\nCrop. Crops the image to it's upper-left height by width region. If a "
                     "parameter exceeds an actual image size, it doesn't affect the image.\n-----\n"
                     "-gs\nGrayscale. Converts the image to shades of gray.\n-----\n"
                     "-neg\nNegative. Inverses the colors of the image.\n-----\n"
                     "-sharp\nSharpening. Applies sharpening to the image.\n-----\n"
                     "-edge threshold\nEdge detection. threshold is a value from 0 to 1. Highlights the edges of the "
                     "image. Higher threshold results in less details preserved.\n-----\n"
                     "-blur sigma\nGaussian blur. sigma is a value higher than 0. Blur the image using the Gaussian "
                     "blur algorithm. Higher sigma results in more intense blur.\n-----\n"
                     "-dist scale strength\nDistortion. scale is a non-zero value from 0 to 1, strength is a non-zero "
                     "value from 0 to 100. Randomly distorts the image using Perlin noise. Higher scale results in "
                     "more locally consistent image, higher strength result in more intense distortion.";
        return 0;
    }
    if (tokens.size() < 3) {
        throw std::invalid_argument("Too few arguments. You have to at least specify input_path, output_path.");
    }
    const std::string read_path = tokens[1].name;
    const std::string write_path = tokens[2].name;
    Image image = ReadImage(read_path);
    image = ApplyFilters(image, tokens);
    WriteImage(write_path, image);
    std::cout << std::time(nullptr) - st;
}