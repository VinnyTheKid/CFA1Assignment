#include "Box.h"
#include <ngl/ShaderLib.h>
#include "ngl/Vec3.h"
#include "ngl/Types.h"



Box::Box()
{
m_width = 20;
m_height = 15;
m_depth = 30;
m_centre = ngl::Vec3(0,0,0);
m_mesh = new ngl::Obj("models/box.obj");
m_mesh->createVAO();
}

void Box::draw(const std::string &_shader,  ngl::Camera *_cam)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

    m_transform.setRotation(ngl::Vec3(0,180,0));
      ngl::Mat4 MVP=m_transform.getMatrix() * _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    m_mesh->draw();
}


