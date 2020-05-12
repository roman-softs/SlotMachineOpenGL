#ifndef WHEEL_H
#define WHEEL_H

#include "Simple3DObject.h"


class Wheel : public Simple3DObject
{
public:
    Wheel();

    void Draw(const Shader &shader, bool use_textures = true);
    void startRoating(float time);
    void stopRoating();
    bool isActive() const;

private:
    bool m_isActive;

    double m_end_time;
    float m_lastFrame;

    float m_speed;


};

#endif // WHEEL_H
