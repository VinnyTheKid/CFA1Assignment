#ifndef BAT_H
#define BAT_H

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>

class Bat
{
public :
  /// @brief ctor
  Bat();
  /// @brief Sets the bat's mouse-controlled position
  /// @param _mousePos the new position
  inline void setMousePos(  const ngl::Vec3 _mousePos){m_mousePos = _mousePos;}
  /// @brief Sets the bat's normal vector
  /// @param _normal the new normal vector
  inline void setNormal( const ngl::Vec3 _normal) {m_normal = _normal;}
  /// @brief Sets the bat's orientation
  /// @param _orient the new orientation
  inline void setOrient(const ngl::Vec3 _orient){m_orientation = _orient;}
  /// @brief Sets the bat's radius
  /// @param _r the new radius
  inline void setRadius( const ngl::Real _r) {m_r=_r;}
  /// @brief returns the bat's true global position
  inline ngl::Vec3 getPos() {return m_pos;}
  /// @brief returns the bat's mouse-controlled position
  inline ngl::Vec3 getMousePos(){return m_mousePos;}
  /// @brief returns the bat's mouse-controlled velocity
  inline ngl::Vec3 getMouseV(){return m_mouseV;}
  /// @brief returns the bat's normal
  inline ngl::Vec3 getNormal() {return m_normal;}
  /// @brief returns the bat's push-controlled velocity
  inline ngl::Vec3 getPushV() {return m_pushV;}
  /// @brief returns the bat's push-controlled acceleration
  inline ngl::Vec3 getPushA() {return m_pushA;}
  /// @brief returns the bat's push-controlled position
  inline ngl::Vec3 getPushPos() {return m_pushPos;}
  /// @brief returns the bat's radius
  inline ngl::Real getRadius() const {return m_r;}
  /// @brief returns a flag for the state of the push command
  inline std::string getPush() {return m_push;}
  /// @brief returns the limit of the reach of a push
  inline ngl::Real getPushDist(){return m_pushDist;}
  /// @brief draws the bat
  /// @param &_shader the shader used to draw
  /// @param *_cam the viewing camera
  void draw(  const std::string &_shader, ngl::Camera *_cam );
  /// @brief initiates the push command
  void pushStart();
  /// @brief initiated when bat has reached push limit
  void pushPeak();
  /// @brief completes the push and returns mouse control
  void pushStop();



private:
  /// @brief true position of the bat
  ngl::Vec3 m_pos;
  /// @brief mouse-controlled position
  ngl::Vec3 m_mousePos;
  /// @brief mouse-controlled velocity
  ngl::Vec3 m_mouseV;
  /// @brief push-controlled position
  ngl::Vec3 m_pushPos;
  /// @brief push-controlled velocity
  ngl::Vec3 m_pushV;
  /// @brief push-controlled acceleration
  ngl::Vec3 m_pushA;
  /// @brief the bat's radius
  ngl::Real m_r;
  /// @brief a pointer to the bat's mesh
  ngl::Obj *m_mesh;
  /// @brief the transformation cause by mouse control
  ngl::Transformation m_mouseTransform;
  /// @brief the transformation cause by the bat aiming at the ball
  ngl::Transformation m_orientTransform;
  /// @brief the transformation cause by a push
  ngl::Transformation m_pushTransform;
  /// @brief the result of the previous 3 transformations for drawing
  ngl::Transformation m_transform;
  /// @brief the bat's orientation
  ngl::Vec3 m_orientation;
  /// @brief the bat's normal
  ngl::Vec3 m_normal;
  /// @brief the limit of the distance covered by a push
  ngl::Real m_pushDist;
  /// @brief a flag to show whether a push is taking place and the bat's phase of said push
  std::string m_push;

};

#endif // BAT_H
