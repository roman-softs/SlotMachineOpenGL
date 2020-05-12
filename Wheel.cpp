#include "Wheel.h"

#include <GLFW/glfw3.h>
#include "random.hpp"

const int angle_texture = 20;


Wheel::Wheel() : Simple3DObject("../SlotMachine/Models/bar.obj", "../SlotMachine/Textures/bar_texture.png"), m_isActive(false), m_end_time(0.0), m_lastFrame(0.0f)
{

}

void Wheel::Draw(const Shader &shader, bool use_textures)
{
    double currentTime =  glfwGetTime();
    float deltaTime = currentTime - m_lastFrame;
    m_lastFrame = currentTime;

    if(glfwGetTime() < m_end_time)
        m_angle = m_angle +  m_speed * deltaTime;
    else
    {
        if(m_isActive)
            m_angle = int(m_angle/angle_texture)*angle_texture + angle_texture;
        m_isActive = false;

    }

    Simple3DObject::Draw(shader, use_textures);
}

void Wheel::startRoating(float time)
{
    m_end_time = glfwGetTime() + time;
    m_isActive = true;
    m_lastFrame = 0.0f;

    m_speed = Random::get(100.0f, 600.0f);
}

void Wheel::stopRoating()
{
    m_isActive = true;
}

bool Wheel::isActive() const
{
    return m_isActive;
}
