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
    void setPosition (ngl::Vec3 _pos);
    inline ngl::Vec3 getPosition(){return m_pos;}
    void setRadius (    const ngl::Real _r);
    inline ngl::Real getRadius(){return m_r;}
    void draw(const std::string &_shader, ngl::Camera *_cam );
    void generatePos(   const ngl::Real _boxWidth, const ngl::Real _boxHeight, const ngl::Real _boxDepth);//this includes setting the orientation needed depending on the wall chosen

private:
    ngl::Vec3 m_pos;
    ngl::Vec3 m_orientation;
    ngl::Real m_r;
    std::string m_source;
    ngl::Obj *m_mesh;
    ngl::Transformation m_transform;

};

#endif // GOAL

