#ifndef SIMPLE3DOBJECT_H
#define SIMPLE3DOBJECT_H


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <stb_image.h>

#include <string>
#include <vector>

#include "Shader.h"

using namespace std;

bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
);

struct VertexData
{
    VertexData(){}
    VertexData(glm::vec3 p, glm::vec3 n, glm::vec2 t) :
        position(p), normal(n), texCoord(t)
    {

    }
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Simple3DObject
{
public:
    Simple3DObject(const vector<VertexData>& vertices,const vector<unsigned int>& indices,const string& texture_path);
    Simple3DObject(const string &obj_path, const string& texture_path);
    virtual ~Simple3DObject();
    virtual void Draw(const Shader &shader, bool use_textures = true);

    void rotate(float angle, const glm::vec3& vec);
    void scale(float scale);
    void translate(const glm::vec3& translate);

protected:
    void init();
    unsigned int TextureFromFile(const string &texture_path);
    void LoadObject(const string &obj_path);

protected:
    vector<VertexData>      m_vertices;
    vector<unsigned int>    m_indices;


    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;

    unsigned int m_textureId;

    glm::vec3 m_rotation;
    float m_angle;


    glm::vec3 m_translation;
    float m_scale;

};

#endif // SIMPLE3DOBJECT_H
