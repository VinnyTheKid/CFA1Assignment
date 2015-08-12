
#include "Bat.h"
#include "Ball.h"
#include <ngl/ShaderLib.h>

Bat::Bat( const ngl::Vec3 _mousePos )
{
    m_mousePos =_mousePos;
    m_pushPos = (0,0,0);
    m_r = 3;
    m_source = "models/bat.obj";
    m_mesh = new ngl::Obj(m_source);
    m_mesh->createVAO();
    m_mouseV = (0,0,0);
    m_pushV = (0,0,0);
    m_normal = (0,0,1);


    m_mouseTransform.setPosition(m_pos);
    m_pushTransform.setRotation(m_orientation);
    m_pushTransform.setPosition(m_pushPos);
    m_transform = m_mouseTransform * m_pushTransform; //order change?

    ngl::Vec3 transMouseToFinal;
    ngl::Real x, y, z, batPushDistance = 0.5;// find a way to bring in static for batpushdist
    x = batPushDistance * sin(m_orientation.m_y);
    y = batPushDistance * sin(m_orientation.m_x);
    z = sqrt(pow(batPushDistance,2)-pow(x,2)-pow(y,2));
    transMouseToFinal = (x, y, z);

    m_pos = m_mousePos + transMouseToFinal;

    ngl::Real m_orientX, m_orientY;
    m_orientX = atan(m_normal.m_x/m_normal.m_z);
    m_orientY = atan(m_normal.m_y/m_normal.m_z);
    m_orientation = (m_orientX, m_orientY, 0);

    ngl::Colour _amb(0.9f,0.9f,0.9f);
    ngl::Colour _dif(9.0f,0.05f,0.1f);
    ngl::Colour _spec(0.50f,0.50f,0.50f);
    ngl::Real _rough(0.15f);

    m_material.setAmbient(_amb);
    m_material.setDiffuse(_dif);
    m_material.setSpecular(_spec);
    m_material.setRoughness(_rough);
    m_push = "False";
}


void Bat::draw(const std::string &_shader,  ngl::Camera *_cam )
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

    ngl::Mat4 MVP= m_mouseTransform.getMatrix() * m_pushTransform.getMatrix() * _cam->getVPMatrix();
    shader->setShaderParamFromMat4("MVP",MVP);

    m_mesh->draw();
}


const static float s_xLimits = 45;
const static float s_yLimits = 30;

void Bat::mouseMove(const float _x, const float _y)
{


    float oldX = m_pos.m_x;
    float oldY = m_pos.m_y;
    m_pos.m_x += _x;
    m_pos.m_y += _y;
    if(m_pos.m_x <= -s_xLimits || m_pos.m_x >= s_xLimits)
    {
        m_pos.m_x = oldX;
    }
    if(m_pos.m_y <= -s_yLimits || m_pos.m_x >= s_yLimits)
    {
        m_pos.m_y = oldY;
    }
}


void Bat::pushStart()
{
    m_push = "TrueTo";
    m_pushA = 0.1;
}

void Bat::pushStop()
{
    m_push = "False";
    m_pushA = (0,0,0);
    m_pushV = (0,0,0);
}

void Bat::pushPeak()
{
    m_push = "TrueFrom";
    ngl::Vec3 newPushV = (0,0,-m_pushV.m_z);
    m_pushV = newPushV;
}
