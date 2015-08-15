
#include "Goal.h"
#include "Box.h"
#include <ngl/ShaderLib.h>
#include <stdlib.h>
#include <time.h>

Goal::Goal()
{
        m_pos = (0,0,0);
        m_scale = 1;
        m_r = 5 * m_scale;
        m_source= "models/goal.obj";
        m_mesh = new ngl::Obj(m_source);
        m_orientation = (0,0,0);
        m_mesh->createVAO();
}
void Goal::setPosition(     const ngl::Vec3 _pos, const ngl::Vec3 _orient)
{
    m_pos = _pos;
    m_orientation = _orient;
}

void Goal::setRadius (    const ngl::Real _r)
{
    m_r = _r;
}

void Goal::draw(const std::string &_shader, ngl::Camera *_cam )

{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

      m_transform.setScale(ngl::Vec3(1,1,m_scale));
      m_transform.setRotation(m_orientation);
      m_transform.setPosition(m_pos);
      ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    m_mesh->draw();
}

void Goal::generatePos()
{
    ngl::Real boxWidth=20, boxHeight=15, boxDepth=30;

    ngl::Vec3 newGoalPos;
    m_orientation = (0,180,0);

    ngl::Real minX, maxX, minY, maxY, x, y, z;
    minX = m_r-(boxWidth/2);
    maxX = (boxWidth/2)-m_r;
    minY = m_r-(boxHeight/2);
    maxY = (boxHeight/2)-m_r;
    z = boxDepth/2;

    float a = generateFloat(minX,maxX);
    float b = generateFloat(minY,maxY);

    x = a; y = b;
    newGoalPos = (x, y, z);
    m_pos = newGoalPos;

}

int Goal::generateInt(const int _min, const int _max)
{
    return rand() % _max + _min;
}

float Goal::generateFloat(const float _min, const float _max)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float r = random * (_min - _max);
    return _min + r;
}
