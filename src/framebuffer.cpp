#include "framebuffer.hpp"
using namespace rayster;

#include <algorithm>
#include <fstream>

void ppm_framebuffer::write(const std::string& path) const {
    std::ofstream file(path);
    file << "P3\n";
    file << width() << ' ' << height() << '\n';
    file << "255\n";

    auto y = height();
    for (size_type y = 0; y < height(); ++y) {
        for (size_type x = 0; x < width(); ++x) {
            auto r = static_cast<int>((*this)(x, y).color.r * 255);
            auto g = static_cast<int>((*this)(x, y).color.g * 255);
            auto b = static_cast<int>((*this)(x, y).color.b * 255);

            r = std::clamp(r, 0, 255);
            g = std::clamp(g, 0, 255);
            b = std::clamp(b, 0, 255);

            file << r << ' ';
            file << g << ' ';
            file << b << ' ';
        }
    }
}

void pam_framebuffer::write(const std::string& path) const {
    std::ofstream file(path);
    file << "P7\n";
    file << "WIDTH " << width() << '\n';
    file << "HEIGHT " << height() << '\n';
    file << "DEPTH 4\n";
    file << "MAXVAL 255\n";
    file << "TUPLTYPE RGB_ALPHA\n";
    file << "ENDHDR\n";

    auto y = height();
    for (size_type y = 0; y < height(); ++y) {
        for (size_type x = 0; x < width(); ++x) {
            auto r = static_cast<int>((*this)(x, y).color.r * 255);
            auto g = static_cast<int>((*this)(x, y).color.g * 255);
            auto b = static_cast<int>((*this)(x, y).color.b * 255);
            auto a = static_cast<int>((*this)(x, y).color.a * 255);

            r = std::clamp(r, 0, 255);
            g = std::clamp(g, 0, 255);
            b = std::clamp(b, 0, 255);
            a = std::clamp(a, 0, 255);

            file.put(r); file.put(g); file.put(b); file.put(a);
        }
    }
}