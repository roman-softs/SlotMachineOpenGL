#include "StartButton.h"
#include <GLFW/glfw3.h>

StartButton::StartButton()  : Simple3DObject("../SlotMachine/Models/start_button.obj", "../SlotMachine/Textures/start_button.png"), m_isEnable(true)
{

}

void StartButton::Draw(const Shader &shader, bool use_textures)
{
    if(m_isEnable)
    {
        float timeValue = glfwGetTime();
        float lightValue = sin(timeValue) / 2.0f + 0.5f;
        shader.setVec3("light.ambient", lightValue, lightValue, lightValue);
    }

    Simple3DObject::Draw(shader, use_textures);
    if(m_isEnable)
        shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
}

void StartButton::setEnable(bool b)
{
    m_isEnable = b;
}

bool StartButton::isEnable() const
{
    return m_isEnable;
}
