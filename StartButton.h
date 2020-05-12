#ifndef STARTBUTTON_H
#define STARTBUTTON_H

#include "Simple3DObject.h"

class StartButton : public Simple3DObject
{
public:
    StartButton();
    void Draw(const Shader &shader, bool use_textures = true);

    void setEnable(bool b);
    bool isEnable() const;

private:
    bool m_isEnable;
};

#endif // STARTBUTTON_H
