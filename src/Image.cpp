#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>

Image::Image(const std::string& path) {
    unsigned char* data = stbi_load(
        path.c_str(),
        &m_width,
        &m_height,
        &m_channels,
        0
    );

    if (!data) {
        throw std::runtime_error("Failed to load image");
    }

    size_t size = m_width * m_height * m_channels;
    m_pixels.assign(data, data + size);

    stbi_image_free(data);
}

void Image::save(const std::string& path) {
    stbi_write_png(
        path.c_str(),
        m_width,
        m_height,
        m_channels,
        m_pixels.data(),
        m_width * m_channels
    );
}
