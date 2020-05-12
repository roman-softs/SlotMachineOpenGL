#include "Simple3DObject.h"
#include "obj_load.h"

Simple3DObject::Simple3DObject(const vector<VertexData>& vertices,const vector<unsigned int>& indices,const string& texture_path) :
     m_rotation(glm::vec3(0.5f, 0.0f, 0.0f)), m_angle(0.0f), m_translation(glm::vec3(0.0f)), m_scale(1.0f)
{
    m_vertices = vertices;
    m_indices = indices;

    TextureFromFile(texture_path);
    init();

}

Simple3DObject::Simple3DObject(const string& obj_path, const string &texture_path) :
    m_rotation(glm::vec3(0.5f, 0.0f, 0.0f)), m_angle(0.0f), m_translation(glm::vec3(0.0f)), m_scale(1.0f)
{
    LoadObject(obj_path);

    TextureFromFile(texture_path);
    init();
}

Simple3DObject::~Simple3DObject()
{

}

void Simple3DObject::Draw(const Shader& shader, bool use_textures)
{
    // bind appropriate textures
    /*unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = m_textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
         else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }*/


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(m_angle), m_rotation);
    model = glm::translate(model, m_translation);
    model = glm::scale(model, glm::vec3(m_scale));

    shader.setMat4("model", model);



    if(use_textures)
    {
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader.ID, "TexCoords"), 0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if(use_textures)
        glActiveTexture(GL_TEXTURE0);
}


void Simple3DObject::rotate(float angle, const glm::vec3 &vec)
{
    m_angle = angle;
    m_rotation = vec;
}

void Simple3DObject::scale(float scale)
{
    m_scale = scale;
}

void Simple3DObject::translate(const glm::vec3 &translate)
{
    m_translation = translate;
}

void Simple3DObject::init()
{

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexData), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, texCoord));

   /* glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Bitangent));*/

    glBindVertexArray(0);
}

unsigned int Simple3DObject::TextureFromFile(const string& texture_path)
{
    unsigned int textureID;
    glGenTextures(1, &m_textureId);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void Simple3DObject::LoadObject(const string& obj_path)
{
    // Initialize Loader
    objl::Loader Loader;
    // Load .obj File
    bool loadout = Loader.LoadFile(obj_path.c_str());

    vector<VertexData> vertices;
    vector<unsigned int> indices;

    for (unsigned long long i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        // Copy one of the loaded meshes to be our current mesh
        objl::Mesh curMesh = Loader.LoadedMeshes[i];

        for (unsigned long long j = 0; j < curMesh.Vertices.size(); j++)
        {
            vertices.push_back(VertexData(glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z),
                                          glm::vec3( curMesh.Vertices[j].Normal.X,  curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z),
                                          glm::vec2( curMesh.Vertices[j].TextureCoordinate.X,  curMesh.Vertices[j].TextureCoordinate.Y)));
        }
        //	triangle that these indices represent
        for (unsigned long long j = 0; j < curMesh.Indices.size(); j ++)
        {
            indices.push_back(curMesh.Indices[j]);
        }


    }
    //return new Simple3DObject(vertices, indices, "../SlotMachine/container.jpg");
    m_vertices = vertices;
    m_indices = indices;

}
