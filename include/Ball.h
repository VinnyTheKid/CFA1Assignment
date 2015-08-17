#ifndef BALL_H
#define BALL_H

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <stdlib.h>
#include <random>
#include <time.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file Ball.h
/// @brief The Ball class, used to define the ball's attributes and access methods.
/// @author Vincent Clifton
/// @version 1.0
/// @date 17/8/2015
/// @class Ball
/// @brief The Ball class, used to define the ball's attributes and access methods.
//----------------------------------------------------------------------------------------------------------------------


class Ball
{
public :
  /// @brief ctor
  Ball();
  /// @brief Sets the ball's position
  /// @param _pos the new position
  inline void setPosition(  const ngl::Vec3 _pos){m_pos = _pos;}
  /// @brief Sets the ball's velocity
  /// @param _v the new velocity
  inline void setVelocity(   const ngl::Vec3 _v) {m_v=_v;}
  /// @brief Sets the ball's radius
  /// @param _r the new radius
  inline void setRadius( const ngl::Real _r) {m_r=_r;}
  /// @brief Returns the ball's position
  inline ngl::Vec3 getPosition() const {return m_pos;}
  /// @brief Returns the ball's velocity
  inline ngl::Vec3 getVelocity() const {return m_v;}
  /// @brief Returns the ball's radius
  inline ngl::Real getRadius() const {return m_r;}
  /// @brief Returns the ball's material
  inline ngl::Material getMaterial() const {return m_material;}
  /// @brief draws the mesh using the transform stack
  /// @param &_shader the shader used to draw
  /// @param *_cam the viewing camera
  void draw(const std::string &_shader, ngl::Camera *_cam );
  /// @brief Uses a random float generator to give the ball a new position
  /// @param _boxWidth width of the box
  void generatePos(ngl::Real _boxWidth);
  /// @brief Uses a random float generator to give the ball a new velocity
  void generateVel();


private:
  /// @brief position of the ball
  ngl::Vec3 m_pos;
  /// @brief velocity of the ball
  ngl::Vec3 m_v;
  /// @brief radius of the ball
  ngl::Real m_r;
  /// @brief pointer to mesh of the ball
  ngl::Obj *m_mesh;
  /// @brief transformation stack of the ball
  ngl::Transformation m_transform;
  /// @brief material of the ball
  ngl::Material m_material;
};

#endif // BALL_H

