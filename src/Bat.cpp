
#include "Bat.h"
#include "Ball.h"
#include <ngl/ShaderLib.h>

Bat::Bat()
{
    m_mousePos = ngl::Vec3(0,0,0);
    m_pushPos = ngl::Vec3(0,0,0);
    m_r = 3;
    m_mesh = new ngl::Obj("models/bat.obj");
    m_mesh->createVAO();
    m_pushV = ngl::Vec3(0,0,0);
    m_defaultNormal = ngl::Vec3(0,0,1);
    m_normal = m_defaultNormal;
    m_pushDist = ngl::Real(1);
    m_orientation = ngl::Vec3(0,0,0);

    ngl::Vec3 transMouseToFinal;
    ngl::Real x, y, z;
    x = m_pushDist * sin(m_orientation.m_y);
    y = m_pushDist * sin(m_orientation.m_x);
    z = sqrt(pow(m_pushDist,2)-pow(x,2)-pow(y,2));
    transMouseToFinal = (x, y, z);

    m_pos = m_mousePos + transMouseToFinal;

    ngl::Colour _amb(0.3f,0.4f,0.9f);
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

    m_mouseTransform.setPosition(m_mousePos);
    m_pushTransform.setRotation(m_orientation.m_x,m_orientation.m_y,m_orientation.m_z);
    //m_pushTransform.setPosition(m_pushPos);


    ngl::Mat4 MVP= m_pushTransform.getMatrix() *m_mouseTransform.getMatrix() * _cam->getVPMatrix();
    shader->setShaderParamFromMat4("MVP",MVP);

    m_mesh->draw();
}




void Bat::pushStart()
{
    m_push = "TrueTo";
    m_pushA = 0.1;
}

void Bat::pushStop()
{
    m_push = "False";
    m_pushA = ngl::Vec3(0,0,0);
    m_pushV = ngl::Vec3(0,0,0);
}

void Bat::pushPeak()
{
    m_push = "TrueFrom";
    ngl::Vec3 newPushV = (0,0,-m_pushV.m_z);
    m_pushV.set(newPushV);
}
