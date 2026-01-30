#pragma once

#include <string>
#include <vector>

class Image {
public:
    Image(const std::string& path);
    Image(int width, int height, int channels, std::vector<unsigned char>& pixels);
    Image(int width, int height, int channels, std::vector<unsigned char>&& pixels);


    void save(const std::string& path);

    int width() const { return m_width; }
    int height() const { return m_height; }
    int channels() const { return m_channels; }

    const std::vector<unsigned char>& pixels() const { return m_pixels; }

private:
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

    std::vector<unsigned char> m_pixels;
};
