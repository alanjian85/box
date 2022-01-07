#include "pam_framebuffer.hpp"
using namespace rayster;

#include <algorithm>
#include <fstream>

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
    do {
        --y;
        for (auto x = 0; x < width(); ++x) {
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
    } while (y > 0);
}