#include "WheelsController.h"
#include "random.hpp"

const int wheels_num = 5;

WheelsController::WheelsController() :
    m_isActive(false)
{
    for(int i=0; i<wheels_num; ++i)
    {
        m_wheels.push_back(Wheel());
    }

    const float wheel_width = 0.4f;
    const float x_between = 0.04f;
    float x_pos = 0.0f - (float(wheel_width*(wheels_num-1)) + float(wheel_width*(wheels_num-1))*x_between)/2.0f;
    for (auto& wheel : m_wheels)
    {
        wheel.translate(glm::vec3(x_pos, 0.0f, 0.0f));
        x_pos += x_between + wheel_width;
    }

    m_button = std::make_shared<StartButton>();
    m_button->translate(glm::vec3(0.0f, -0.7f, 1.3f));
    m_button->scale(0.5f);
}

void WheelsController::Draw(const Shader &shader, bool use_textures)
{
    if(!use_textures)
        shader.setFloat("u_code", 200.0f);
    for (auto& wheel : m_wheels)
        wheel.Draw(shader, use_textures);

    if(!use_textures)
        shader.setFloat("u_code", 100.0f);
    m_button->Draw(shader, use_textures);
}

void WheelsController::update()
{
    std::list<Wheel>::iterator it = std::prev(m_wheels.end());

    if( !((*it).isActive()) )
    {
        m_isActive = false;
        m_button->setEnable(true);
    }
}

void WheelsController::start()
{
    if(!m_isActive)
    {
        m_button->setEnable(false);
        float min_time = Random::get(4.0f, 5.0f);
        for (auto& wheel : m_wheels)
        {
            wheel.startRoating(min_time);
            min_time += Random::get(0.0f, 1.0f);
        }
        m_isActive = true;
    }
}
