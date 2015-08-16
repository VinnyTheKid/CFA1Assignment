#ifndef GOAL
#define GOAL

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include <cstdlib>

class Goal
{
public:
    Goal();
    void setPosition (ngl::Vec3 _pos,ngl::Vec3 _orient);
    inline ngl::Vec3 getPosition(){return m_pos;}
    void setRadius (    const ngl::Real _r);
    inline ngl::Real getRadius(){return m_r;}
    void draw(const std::string &_shader, ngl::Camera *_cam );
    void generatePos(ngl::Real _boxWidth, ngl::Real _boxHeight, ngl::Real _boxDepth);



private:
    ngl::Vec3 m_pos;
    ngl::Vec3 m_orientation;
    ngl::Real m_scale;
    ngl::Real m_r;
    std::string m_source;
    ngl::Obj *m_mesh;
    ngl::Transformation m_transform;

};

#endif // GOAL

