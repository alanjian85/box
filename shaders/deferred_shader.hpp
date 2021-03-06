#ifndef BOX_DEFERRED_SHADER_HPP
#define BOX_DEFERRED_SHADER_HPP

#include "light.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace box {
    class deferred_shader : public basic_shader {
    public:
        deferred_shader(matrix4 view, const model& mesh, directional_light light, matrix4 light_proj, matrix4 light_view, const texture& shadowmap) {
            view_ = view;
            mesh_ = &mesh;
            light_ = light;

            light_proj_ = light_proj;
            light_view_ = light_view;
            shadowmap_sampler_.bind_texture(shadowmap);
        }

        void set_position_buffer(const texture& g_position) {
            position_sampler_.bind_texture(g_position);
        }

        void set_normal_buffer(const texture& g_normal) {
            normal_sampler_.bind_texture(g_normal);
        }

        void set_ambient_buffer(const texture& g_ambient) {
            ambient_sampler_.bind_texture(g_ambient);
        }

        void set_albedo_buffer(const texture& g_albedo) {
            albedo_sampler_.bind_texture(g_albedo);
        }

        void set_specular_buffer(const texture& g_specular) {
            specular_sampler_.bind_texture(g_specular);
        }

        void set_emission_buffer(const texture& g_emission) {
            emission_sampler_.bind_texture(g_emission);
        }

        virtual vector4 vert(int n) override {
            auto pos = mesh_->get_vertex(n);
            v_uv_[n % 3] = mesh_->get_uv(n);
            return vector4(pos, 1);
        }

        virtual std::optional<color_rgba> frag(vector3 bar) override {
            auto uv = frag_lerp(v_uv_, bar);

            auto position = vector3(position_sampler_(uv));
            auto normal = vector3(normal_sampler_(uv));
            auto cam_dir = -position.normalize();
            auto light_dir = -vector3(view_ * vector4(light_.dir, 1)).normalize();
            auto halfway_dir = (cam_dir + light_dir).normalize();

            auto offset = vector3(view_(0, 3), view_(1, 3), view_(2, 3));
            auto pos_light_space = light_proj_ * light_view_ * vector4(matrix3(view_).transpose() * (position - offset), 1);
            auto pos_proj_coord = vector3(pos_light_space) / pos_light_space.w;
            pos_proj_coord = pos_proj_coord * 0.5 + 0.5;
            auto current_depth = pos_proj_coord.z;
            auto bias = 0.005;
            auto shadow = 0.0;
            auto& shadowmap = shadowmap_sampler_.get_texture();
            auto texel_size = vector2(1.0 / shadowmap.width(), 1.0 / shadowmap.height());
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    auto depth = shadowmap_sampler_(vector2(pos_proj_coord) + vector2(x, y) * texel_size).r;
                    shadow += current_depth - bias > depth ? 1.0 : 0.0;
                }
            }
            shadow /= 9;

            auto ambient = color_rgb(albedo_sampler_(uv)) * color_rgb(ambient_sampler_(uv));
            auto diffuse = light_.diffuse * color_rgb(albedo_sampler_(uv)) * color_rgb(std::max(dot(light_dir, normal), 0.0));
            auto specular = light_.specular * color_rgb(specular_sampler_(uv)) * color_rgb(std::pow(std::max(dot(halfway_dir, normal), 0.0), 256));
            auto emission = color_rgb(emission_sampler_(uv)) * 10;

            return color_rgba(ambient + (1 - shadow) * (diffuse + specular) + emission, 1.0);
        }
    private:
        matrix4 view_;
        const model* mesh_;
        vector2 v_uv_[3];

        sampler2 position_sampler_;
        sampler2 normal_sampler_;
        sampler2 ambient_sampler_;
        sampler2 albedo_sampler_;
        sampler2 specular_sampler_;
        sampler2 emission_sampler_;

        directional_light light_;
        matrix4 light_proj_;
        matrix4 light_view_;
        sampler2 shadowmap_sampler_;
    };
}

#endif