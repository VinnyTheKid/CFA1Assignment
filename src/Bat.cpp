
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
  m_pushA = ngl::Vec3(0,0,0);
  m_normal = ngl::Vec3(0,0,1);
  m_pushDist = ngl::Real(1);
  m_orientation = ngl::Vec3(0,0,0);

  ngl::Vec3 transMouseToFinal;
  ngl::Real x, y, z;
  x = m_pushDist * cos(m_orientation.m_z);
  y = m_pushDist * sin(m_orientation.m_z);
  z = m_pushDist * cos(m_orientation.m_x);
  transMouseToFinal = (x, y, z);

  m_pos.set(ngl::Vec3(m_mousePos + transMouseToFinal));

  ngl::Colour _amb(0.3f,0.4f,0.9f);
  ngl::Colour _dif(9.0f,0.05f,0.1f);
  ngl::Colour _spec(0.50f,0.50f,0.50f);
  ngl::Real _rough(0.15f);

  m_push = "False";
}


void Bat::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();

  m_mouseTransform.setPosition(m_mousePos);
  m_orientTransform.setRotation(m_orientation);
  m_pushTransform.setPosition(m_pushPos);


  ngl::Mat4 MVP= m_pushTransform.getMatrix() * m_orientTransform.getMatrix() * m_mouseTransform.getMatrix() * _cam->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

  m_mesh->draw();
}




void Bat::pushStart()
{
  m_push = "TrueTo";
  m_pushA = ngl::Vec3(0,0,0.3f);
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
