#ifndef BALL_H
#define BALL_H

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include <ngl/Material.h>
#include <stdlib.h>
#include <time.h>


class Ball
    {
    public :
        Ball();
        void draw(const std::string &_shader, ngl::Camera *_cam );//called in drawScene
        inline void setPosition(   const ngl::Vec3 _pos ){m_pos = _pos;}//
        inline void setVelocity(   const ngl::Vec3 _v) {m_v=_v;}
        inline void setRadius( const ngl::Real _r) {m_r=_r;}
        inline ngl::Vec3 getPosition() const {return m_pos;}
        inline ngl::Vec3 getVelocity() const {return m_v;}
        inline ngl::Real getRadius() const {return m_r;}
        inline std::string getSource() const {return m_source;}//string containing file location
        inline ngl::Material getMaterial() const {return m_material;}
        ngl::Vec3 batDeflect(ngl::Vec3 _batNormal);//changses the velocity vector when colliding with the bat. Still needs to have 'energy transfer' simulation included
        ngl::Vec3 generatePos(   const ngl::Real _boxWidth, const ngl::Real _boxHeight, const ngl::Real _boxDepth);//make a vec3 to use for position once a goal has been scored
        ngl::Vec3 generateVel();//make a vec3 to use for the starting velocity once a goal has been scored. Needs calibration


    private:
        std::string m_source;
        ngl::Vec3 m_pos;
        ngl::Vec3 m_v;
        ngl::Real m_r;
        ngl::Obj *m_mesh;
        ngl::Transformation m_transform;
        ngl::Material m_material;
    };

#endif // BALL_H

