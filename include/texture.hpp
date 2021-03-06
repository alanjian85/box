#ifndef BOX_TEXTURE_HPP
#define BOX_TEXTURE_HPP

#include <filesystem>
#include <string>
#include <vector>

#include "color.hpp"
#include "vector.hpp"

namespace box {
    class texture {
    public:
        texture() : width_(), height_() {}
        texture(int width, int height)
            : pixels_(width * height),
              width_(width), height_(height)
        {

        }

        void load(const std::string& path);

        int width() const {
            return width_;
        }

        int height() const {
            return height_;
        }

        color_rgba& operator()(int x, int y) {
            auto index = y * width_ + x;
            return pixels_[index];
        }

        const color_rgba& operator()(int x, int y) const {
            auto index = y * width_ + x;
            return pixels_[index];
        }
    private:
        int width_, height_;
        std::vector<color_rgba> pixels_;
    };

    enum class wrapping {
        repeat,
        repeat_mirrored,
        clamp_to_edge,
        clamp_to_border
    };

    class sampler2 {
    public:
        sampler2()
            : wrap_(wrapping::repeat),
              border_{0, 0, 0, 0},
              tex_(nullptr)
        {

        }

        void bind_texture(const texture& tex) {
            tex_ = &tex;
        }

        const texture& get_texture() const {
            return *tex_;
        }

        wrapping wrap() const {
            return wrap_;
        }

        void set_wrap(wrapping wrap) {
            wrap_ = wrap;
        }

        void set_border(color_rgba border) {
            border_ = border;
        }

        color_rgba border() const {
            return border_;
        }

        color_rgba operator()(vector2 uv) const;

        color_rgba operator()(double u, double v) const {
            return (*this)({u, v});
        }
    private:
        wrapping wrap_;
        color_rgba border_;
        const texture* tex_;
    };

    class sampler_cube {
    public:
        sampler_cube() noexcept
            : wrap_s_(wrapping::repeat),
              wrap_t_(wrapping::repeat),
              wrap_r_(wrapping::repeat),
              right_(nullptr),
              left_(nullptr),
              top_(nullptr),
              bottom_(nullptr),
              back_(nullptr),
              front_(nullptr)
        {

        }

        void bind_right(const texture& right) {
            right_ = &right;
        }

        void bind_left(const texture& left) {
            left_ = &left;
        }

        void bind_top(const texture& top) {
            top_ = &top;
        }

        void bind_bottom(const texture& bottom) {
            bottom_ = &bottom;
        }

        void bind_back(const texture& back) {
            back_ = &back;
        }

        void bind_front(const texture& front) {
            front_ = &front;
        }

        wrapping wrap_s() const {
            return wrap_s_;
        }

        void set_wrap_s(wrapping wrap) {
            wrap_s_ = wrap;
        }

        wrapping wrap_t() const {
            return wrap_t_;
        }

        void set_wrap_t(wrapping wrap) {
            wrap_t_ = wrap;
        }

        wrapping wrap_r() const {
            return wrap_r_;
        }

        void set_wrap_r(wrapping wrap) {
            wrap_r_ = wrap;
        }

        void set_border(color_rgba border) {
            border_ = border;
        }

        color_rgba border() const {
            return border_;
        }

        color_rgba operator()(vector3 uvw) const;
    private:
        wrapping wrap_s_;
        wrapping wrap_t_;
        wrapping wrap_r_;
        color_rgba border_;

        const texture* right_;
        const texture* left_;
        const texture* top_;
        const texture* bottom_;
        const texture* back_;
        const texture* front_;
    };

    struct cubemap {
        sampler_cube sampler;
        texture right;
        texture left;
        texture top;
        texture bottom;
        texture back;
        texture front;

        cubemap() {
            sampler.bind_right(right);
            sampler.bind_left(left);
            sampler.bind_top(top);
            sampler.bind_bottom(bottom);
            sampler.bind_back(back);
            sampler.bind_front(front);
        }

        void load(const std::filesystem::path& path) {
            right.load((path / "right.pam").string());
            left.load((path / "left.pam").string());
            top.load((path / "top.pam").string());
            bottom.load((path / "bottom.pam").string());
            back.load((path / "back.pam").string());
            front.load((path / "front.pam").string());
        }
    };
}

#endif