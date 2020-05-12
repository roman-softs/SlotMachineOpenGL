#include "GLScene.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
glm::vec3 lightPos(0.0f, 0.0f, 4.0f);


GLScene::GLScene()
{

}

GLScene::~GLScene()
{
    glDeleteVertexArrays(1, &m_cubeVAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    glDeleteBuffers(1, &m_VBO);

}

void GLScene::render()
{

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.15f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_lightingShader.use();
    m_lightingShader.setVec3("light.position", lightPos);
    m_lightingShader.setVec3("viewPos", m_camera.Position);

    m_lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    m_lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    m_lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    m_lightingShader.setFloat("material.shininess", 64.0f);

    m_projection = glm::perspective(glm::radians(m_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    m_view = m_camera.GetViewMatrix();
    m_lightingShader.setMat4("projection", m_projection);
    m_lightingShader.setMat4("view", m_view);

    m_wheel_ctrl->Draw(m_lightingShader);
    m_wheel_ctrl->update();

    m_corpus->Draw(m_lightingShader, true);

}

void GLScene::selectObject(int x, int y, int width, int height)
{
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_projection = glm::perspective(glm::radians(m_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    m_view = m_camera.GetViewMatrix();
    m_selectShader.use();
    m_selectShader.setMat4("projection", m_projection);
    m_selectShader.setMat4("view", m_view);

    m_selectShader.setFloat("u_code", 100.0f);
    m_wheel_ctrl->Draw(m_selectShader, false);

    m_selectShader.setFloat("u_code", 200.0f);
    m_corpus->Draw(m_lightingShader, true);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    unsigned char res[4];
    glReadPixels(x, viewport[3]-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

    glDisable(GL_DEPTH_TEST);

    if(int(res[0]) == 100)
        m_wheel_ctrl->start();
}

void GLScene::inputSignal()
{
    m_wheel_ctrl->start();
}

Camera &GLScene::getCamera()
{
    return m_camera;
}

void GLScene::init()
{
    m_rotate_bar = 0.0f;

    m_lastX = SCR_WIDTH / 2.0f;
    m_lastY = SCR_HEIGHT / 2.0f;
    m_firstMouse = true;

    m_deltaTime = 0.0f;
    m_lastFrame = 0.0f;

    m_camera.setValues(glm::vec3(0.0f, 0.0f, 3.0f));

    m_lightingShader.init("../SlotMachine/Shaders/lighting_maps.vsh", "../SlotMachine/Shaders/lighting_maps.fsh");
    m_selectShader.init("../SlotMachine/Shaders/select.vsh", "../SlotMachine/Shaders/select.fsh");

    m_lightingShader.use();
    m_lightingShader.setInt("material.diffuse", 0);
    m_lightingShader.setInt("material.specular", 1);

    m_wheel_ctrl = std::make_shared<WheelsController>();
    Shader ourShader("../SlotMachine/Shaders/vertShader.vsh", "../SlotMachine/Shaders/fragShader.fsh");
    m_corpus = std::make_shared<Simple3DObject>("../SlotMachine/Models/corpus.obj", "../SlotMachine/Textures/corpus.jpg");
}


