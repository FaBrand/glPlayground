#include "ObjModel.h"
#include <stdio.h>
#include <cstring>
#include <iostream>

// Coder for simpel obj loader was taken from
// https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp
ObjModel::ObjModel(const std::string& obj_file)
{
    FILE* file = fopen(obj_file.c_str(), "r");
    if (file == NULL)
    {
        std::cout << "Could not open file: " << obj_file << std::endl;
        return;
    }

    while (1)
    {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;  // EOF = End Of File. Quit the loop.

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices_.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y;  // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you
                           // want to use TGA or BMP loaders.
            uvs_.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals_.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string vertex1, vertex2, vertex3;
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file,
                                 "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0],
                                 &uvIndex[0],
                                 &normalIndex[0],
                                 &vertexIndex[1],
                                 &uvIndex[1],
                                 &normalIndex[1],
                                 &vertexIndex[2],
                                 &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9)
            {
                std::cout << "File can't be read by this parser" << std::endl;
                fclose(file);
                return;
            }

            // indices 2 and 1 swappe to invert winding order

            vertex_indices_.push_back(vertexIndex[0] - 1);
            vertex_indices_.push_back(vertexIndex[2] - 1);
            vertex_indices_.push_back(vertexIndex[1] - 1);

            uv_indices_.push_back(uvIndex[0] - 1);
            uv_indices_.push_back(uvIndex[2] - 1);
            uv_indices_.push_back(uvIndex[1] - 1);

            normal_indices_.push_back(normalIndex[0] - 1);
            normal_indices_.push_back(normalIndex[2] - 1);
            normal_indices_.push_back(normalIndex[1] - 1);
        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    fclose(file);
}

const std::vector<glm::vec3>& ObjModel::GetOutVertices()
{
    return vertices_;
}

const std::vector<glm::vec2>& ObjModel::GetOutUvs()
{
    return uvs_;
}

const std::vector<glm::vec3>& ObjModel::GetOutNormals()
{
    return normals_;
}

const std::vector<std::uint32_t>& ObjModel::GetIndicesVertices()
{
    return vertex_indices_;
}

const std::vector<std::uint32_t>& ObjModel::GetIndicesUvs()
{
    return uv_indices_;
}

const std::vector<std::uint32_t>& ObjModel::GetIndicesNormals()
{
    return normal_indices_;
}
