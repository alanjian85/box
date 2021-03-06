#ifndef BOX_MODEL_HPP
#define BOX_MODEL_HPP

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

#include "material.hpp"
#include "vector.hpp"

namespace box {
    class model {
    public:
        model() = default;
        explicit model(const std::filesystem::path& path) {
            load(path);
        }

        void load(const std::filesystem::path& path);

        int num_vertices() const {
            return static_cast<int>(face_vertices_.size());
        }

        vector3 get_vertex(int n) const {
            return vertices_[face_vertices_[n]];
        }

        vector2 get_uv(int n) const {
            return uvs_[face_uvs_[n]];
        }

        vector3 get_normal(int n) const {
            return normals_[face_normals_[n]];
        }

        const material* get_mat(int n) const {
            for (auto& mat : materials_) {
                if (n >= mat.first) {
                    return mat.second;
                }
            }
            return nullptr;
        }
    private:
        std::vector<vector3> vertices_;
        std::vector<vector2> uvs_;
        std::vector<vector3> normals_;

        std::vector<std::size_t> face_vertices_; 
        std::vector<std::size_t> face_normals_;
        std::vector<std::size_t> face_uvs_;
    
        material_library mtllib_;
        std::vector<std::pair<std::size_t, const material*>> materials_;
    };
}

#endif