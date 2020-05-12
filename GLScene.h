#ifndef GLSCENE_H
#define GLSCENE_H

#include <iostream>

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "glad/glad.h"
#include <cmath>
#include "Shader.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Simple3DObject.h"
#include "WheelsController.h"

#include <shared_mutex>

class GLScene
{
public:
    GLScene();
    ~GLScene();
    void init();
    void render();
    void selectObject(int x, int y, int width, int height);

    void inputSignal();

    Camera& getCamera();

private:
    Shader m_lightingShader;
    Shader m_selectShader;

    std::shared_ptr<WheelsController> m_wheel_ctrl;
    std::shared_ptr<Simple3DObject> m_corpus;

    Camera m_camera;
    float m_lastX;
    float m_lastY;
    bool m_firstMouse;

    // timing
    float m_deltaTime;
    float m_lastFrame;

    unsigned int m_VBO, m_cubeVAO;
    unsigned int m_lightVAO;

    float m_rotate_bar;

    glm::mat4 m_projection;
    glm::mat4 m_view;
};

#endif // GLSCENE_H
