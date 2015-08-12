
#include "Goal.h"
#include "Box.h"
#include <ngl/ShaderLib.h>
#include <stdlib.h>
#include <time.h>

Goal::Goal()
{
        m_pos = (0,0,0);
        m_r = 0.2;
        m_source= "models/goal.obj";
        m_mesh = new ngl::Obj(m_source);
        m_mesh->createVAO();
}
void Goal::setPosition(     const ngl::Vec3 _pos)
{
    m_pos = _pos;
}

void Goal::setRadius (    const ngl::Real _r)
{
    m_r = _r;
}

void Goal::draw(const std::string &_shader, ngl::Camera *_cam )

{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

      m_transform.setPosition(m_pos);
      ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    m_mesh->draw();
}

void Goal::generatePos(   const ngl::Real _boxWidth, const ngl::Real _boxHeight, const ngl::Real _boxDepth)
{
    int wallNo = rand() % 3 + 1;
    ngl::Vec3 newGoalPos;
    if(wallNo ==1)
    {
        m_orientation = (0,90,0);

        ngl::Real minY, maxY, minZ, maxZ, x, y, z;
        x = -_boxWidth/2;
        minY = m_r-(_boxHeight/2);
        maxY = (_boxHeight/2)-m_r;
        minZ = m_r-(_boxDepth/2);
        maxZ = (_boxDepth/2)-m_r;

        float b = minY + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxY - minY));
        float c = minZ + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxZ - minZ));

        y = b; z = c;
        newGoalPos = (x, y, z);
        m_pos = newGoalPos;
    }
    else if(wallNo ==2)
    {
        m_orientation = (0,0,0);

        ngl::Real minX, maxX, minY, maxY, x, y, z;
        minX = m_r-(_boxWidth/2);
        maxX = (_boxWidth/2)-m_r;
        minY = m_r-(_boxHeight/2);
        maxY = (_boxHeight/2)-m_r;
        z = _boxDepth/2;

        float a = minX + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxX - minX));
        float b = minY + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxY - minY));

        x = a; y = b;
        newGoalPos = (x, y, z);
        m_pos = newGoalPos;
    }
    else
    {
        m_orientation = (0,-90,0);

        ngl::Real minY, maxY, minZ, maxZ, x, y, z;
        x = _boxWidth/2;
        minY = m_r-(_boxHeight/2);
        maxY = (_boxHeight/2)-m_r;
        minZ = m_r-(_boxDepth/2);
        maxZ = (_boxDepth/2)-m_r;

        float b = minY + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxY - minY));
        float c = minZ + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(maxZ - minZ));

        y = b; z = c;
        newGoalPos = (x, y, z);
        m_pos = newGoalPos;
    }
}
