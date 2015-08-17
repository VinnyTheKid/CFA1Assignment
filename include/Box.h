#ifndef BOX_H
#define BOX_H

#include "ngl/Types.h"
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>


class Box
{
public :
  Box();
  inline ngl::Vec3 getCentre(){return m_centre;}
  inline ngl::Real getWidth(){return m_width;}
  inline ngl::Real getHeight(){return m_height;}
  inline ngl::Real getDepth(){return m_depth;}
  void draw(const std::string &_shader,  ngl::Camera *_cam);
protected:
  ngl::Obj *m_mesh;
  ngl::Vec3 m_centre;
  ngl::Real m_width, m_height, m_depth;
  ngl::Transformation m_transform;

};

#endif // BOX_H
