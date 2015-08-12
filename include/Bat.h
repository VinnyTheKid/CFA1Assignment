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
        Bat(   const ngl::Vec3 _mousePos);
        void draw(  const std::string &_shader, ngl::Camera *_cam );
        inline void setMousePos(  const ngl::Vec3 _mousePos){m_mousePos = _mousePos;}//the position controlled by mouse input (XY plane)
        inline ngl::Vec3 getMousePos(){return m_pos;}//
        void mouseMove(  const float _x, const float _y);//have a think
        void pushStart();//initiate a 'push', translating the bat through its normal a certain distance
        void pushPeak();//initiate the return of the bat to its original position at mousePos
        void pushStop();//bring the bat to a halt at mousePos and reallow mouse control
        inline void setMouseV(   const ngl::Vec3 _mouseV) {m_mouseV=_mouseV;}//unsure if will need
        inline ngl::Vec3 getMouseV() {return m_mouseV;}//unsure if will need
        inline void setRadius( const ngl::Real _r) {m_r=_r;}
        inline void setNormal( const ngl::Vec3 _normal) {m_normal = _normal;}
        inline ngl::Vec3 getNormal() {return m_normal;}
        inline ngl::Real getRadius() const {return m_r;}
        inline void setPushV(   const ngl::Vec3 _pushV) {m_pushV=_pushV;}//the rate of change of setPushPos
        inline ngl::Vec3 getPushV() {return m_pushV;}
        inline void setPushA(   const ngl::Vec3 _pushA) {m_pushA=_pushA;}//the rate of change of setPushV. The bat accelerates towards a point and then returns as it came.
        inline ngl::Vec3 getPushA() {return m_pushA;}
        inline void setPushPos(   const ngl::Vec3 _pushPos) {m_pushPos=_pushPos;}//Vec3 translates bat along a local Z axis to push. Factors into final transform of bat.
        inline ngl::Vec3 getPushPos() {return m_pushPos;}
        inline std::string getPush() {return m_push;}
        inline ngl::Vec3 getPos() {return m_pos;}//a compilation of all the transforms into global space for collision detection
        inline ngl::Material getMaterial() {return m_material;}


    private:
        std::string m_source; //the obj file location
        ngl::Vec3 m_pos;//the true position from origin
        ngl::Vec3 m_mousePos;//the position controlled by the mouse
        ngl::Vec3 m_pushPos;//the position controlled by the push command
        ngl::Vec3 m_mouseV;//the velocity controlled by the mouse
        ngl::Vec3 m_pushV;//the velocity controlled by the push command
        ngl::Vec3 m_pushA;//the acceleration when push command called
        ngl::Real m_r;//the circle radius for collision detection
        ngl::Obj *m_mesh; //the obj file pointer
        ngl::Transformation m_mouseTransform;//transform stack for mouse movements
        ngl::Transformation m_pushTransform;//transform stack for push command
        ngl::Transformation m_transform;//transform stack for final drawing
        ngl::Vec3 m_orientation;
        ngl::Vec3 m_normal;
        ngl::Material m_material;//the material to be used when rendering
        std::string m_push;//flag to indicate whether a push is already occuring

    };

#endif // BAT_H
