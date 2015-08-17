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
  Bat();
  void draw(  const std::string &_shader, ngl::Camera *_cam );
  inline void setMousePos(  const ngl::Vec3 _mousePos){m_mousePos = _mousePos;}
  inline void setMouseV(  const ngl::Vec3 _mouseV){m_mouseV = _mouseV;}
  inline void setNormal( const ngl::Vec3 _normal) {m_normal = _normal;}
  inline void setRadius( const ngl::Real _r) {m_r=_r;}
  inline void setPushV(   const ngl::Vec3 _pushV) {m_pushV=_pushV;}
  inline void setPushA(   const ngl::Vec3 _pushA) {m_pushA=_pushA;}
  inline void setPushPos(   const ngl::Vec3 _pushPos) {m_pushPos=_pushPos;}
  inline void setOrient(const ngl::Vec3 _orient){m_orientation = _orient;}
  inline ngl::Vec3 getPos() {return m_pos;}
  inline ngl::Vec3 getMousePos(){return m_mousePos;}
  inline ngl::Vec3 getMouseV(){return m_mouseV;}
  inline ngl::Vec3 getNormal() {return m_normal;}
  inline ngl::Real getRadius() const {return m_r;}
  inline ngl::Vec3 getPushV() {return m_pushV;}
  inline ngl::Vec3 getPushA() {return m_pushA;}
  inline ngl::Vec3 getPushPos() {return m_pushPos;}
  inline std::string getPush() {return m_push;}
  inline ngl::Material getMaterial() {return m_material;}
  inline ngl::Real getPushDist(){return m_pushDist;}
  void pushStart();
  void pushPeak();
  void pushStop();



private:
  ngl::Vec3 m_pos;//the true position from origin
  ngl::Vec3 m_mousePos;//the position controlled by the mouse
  ngl::Vec3 m_pushPos;//the position controlled by the push command
  ngl::Vec3 m_mouseV;//the velocity controlled by the mouse
  ngl::Vec3 m_pushV;//the velocity controlled by the push command
  ngl::Vec3 m_pushA;//the acceleration when push command called
  ngl::Real m_r;//the circle radius for collision detection
  ngl::Obj *m_mesh; //the obj file pointer
  ngl::Transformation m_mouseTransform;//transform stack for mouse movements
  ngl::Transformation m_orientTransform;//transform stack for bat rotation
  ngl::Transformation m_pushTransform;//transform stack for push command
  ngl::Transformation m_transform;//transform stack for final drawing
  ngl::Vec3 m_orientation;
  ngl::Vec3 m_normal;
  ngl::Material m_material;//the material to be used when rendering
  ngl::Real m_pushDist;
  std::string m_push;//flag to indicate whether a push is already occuring

};

#endif // BAT_H
