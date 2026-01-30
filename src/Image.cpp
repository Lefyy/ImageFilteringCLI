#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>

namespace {
    std::vector<unsigned char> loadImageData(const std::string& path, int& width, int& height, int& channels) {
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        std::vector<unsigned char> pixels(data, data + width * height * channels);
        stbi_image_free(data);

        return pixels;
    }
}

Image::Image(const std::string& path) 
    : m_pixels(loadImageData(path, m_width, m_height, m_channels))
{}

Image::Image(int width, int height, int channels, const std::vector<unsigned char>& pixels)
    : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels)
{}

Image::Image(int width, int height, int channels, std::vector<unsigned char>&& pixels)
    : m_width(width), m_height(height), m_channels(channels), m_pixels(std::move(pixels))
{}


void Image::save(const std::string& path) const {
    stbi_write_png(
        path.c_str(),
        m_width,
        m_height,
        m_channels,
        m_pixels.data(),
        m_width * m_channels
    );
}
