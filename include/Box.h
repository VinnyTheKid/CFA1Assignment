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
  /// @brief ctor
  Box();
  /// @brief returns the box's width
  inline ngl::Real getWidth(){return m_width;}
  /// @brief returns the box's height
  inline ngl::Real getHeight(){return m_height;}
  /// @brief returns the box's depth
  inline ngl::Real getDepth(){return m_depth;}
  /// @brief draws the box's mesh using the transform stack
  /// @param &_shader the shader used to draw
  /// @param *_cam the viewing camera
  void draw(const std::string &_shader,  ngl::Camera *_cam);

private:
  /// @brief pointer to mesh of the box
  ngl::Obj *m_mesh;
  /// @brief the box's centre position
  ngl::Vec3 m_centre;
  /// @brief the box's width
  ngl::Real m_width;
  /// @brief the box's height
  ngl::Real m_height;
  /// @brief the box's depth
  ngl::Real m_depth;
  /// @brief the box's transform stack for drawing
  ngl::Transformation m_transform;

};

#endif // BOX_H
