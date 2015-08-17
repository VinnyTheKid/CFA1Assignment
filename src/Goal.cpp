
#include "Goal.h"
#include "Box.h"
#include <ngl/ShaderLib.h>
#include <stdlib.h>
#include <time.h>

Goal::Goal()
{
  m_pos = (0,0,0);
  m_scale = 1;
  m_r = 8 * m_scale;
  m_source= "models/goal.obj";
  m_mesh = new ngl::Obj(m_source);
  m_orientation = (0,0,0);
  m_mesh->createVAO();
  srand(time(NULL));
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

  m_transform.setScale(ngl::Vec3(m_scale,m_scale,1));
  m_transform.setRotation(m_orientation);
  m_transform.setPosition(m_pos);
  ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);

  m_mesh->draw();
}

void Goal::generatePos(ngl::Real _boxWidth, ngl::Real _boxHeight, ngl::Real _boxDepth)
{

  float minX = (float)m_r-(_boxWidth/2);
  float maxX = (_boxWidth/2)-(float)m_r;
  float random1 = ((float) rand()) / (float) RAND_MAX;
  ngl::Real posX = minX + (random1 * (maxX - minX));

  float minY = (float)m_r-(_boxHeight/2);
  float maxY = (_boxHeight/2)-(float)m_r;
  float random2 = ((float) rand()) / (float) RAND_MAX;
  float posY = minY + (random2 * (maxY - minY));

  ngl::Real posZ = (_boxDepth/2)-1;

  ngl::Vec3 newPos;
  newPos.set(ngl::Vec3(posX,posY,posZ));
  m_pos = newPos;

}

