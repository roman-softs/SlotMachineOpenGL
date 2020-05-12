#ifndef WHEELSCONTROLLER_H
#define WHEELSCONTROLLER_H

#include "Wheel.h"
#include "StartButton.h"
#include <list>
#include <shared_mutex>

class WheelsController
{
public:
    WheelsController();

    void Draw(const Shader &shader, bool use_textures = true);
    void update();
    void start();

private:
    std::list<Wheel> m_wheels;
    bool m_isActive;

    std::shared_ptr<StartButton> m_button;
};

#endif // WHEELSCONTROLLER_H
