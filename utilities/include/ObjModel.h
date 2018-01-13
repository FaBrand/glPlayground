#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class ObjModel
{

  public:
    ObjModel(const std::string& obj_file);

    const std::vector<glm::vec3>& GetOutVertices();
    const std::vector<glm::vec2>& GetOutUvs();
    const std::vector<glm::vec3>& GetOutNormals();

    const std::vector<std::uint32_t>& GetIndicesVertices();
    const std::vector<std::uint32_t>& GetIndicesUvs();
    const std::vector<std::uint32_t>& GetIndicesNormals();

  private:
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec2> uvs_;
    std::vector<glm::vec3> normals_;
    std::vector<std::uint32_t> vertex_indices_;
    std::vector<std::uint32_t> uv_indices_;
    std::vector<std::uint32_t> normal_indices_;
};
#endif /* OBJMODEL_H */
