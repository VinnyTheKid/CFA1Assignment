#ifndef BOX_H
#define BOX_H

#include "ngl/Types.h"
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>
#include "ngl/VertexArrayObject.h"



//THIS CLASS IS UNFINISHED. Will enable drawing of a VAO box using the parameters

class Box
    {
    public :
        Box(ngl::Vec3 _centre, ngl::Real _width, ngl::Real _height, ngl::Real _depth);
        inline ngl::Vec3 getCentre(){return m_centre;}
        inline ngl::Real getWidth(){return m_width;}
        inline ngl::Real getHeight(){return m_height;}
        inline ngl::Real getDepth(){return m_depth;}
        void draw(const std::string &_shader,  ngl::Camera *_cam);
    protected:
        void setVAO();
        ngl::VertexArrayObject *m_vao;
        GLuint m_drawMode;
        ngl::Vec3 m_centre;
        ngl::Real m_width, m_height, m_depth;
        ngl::Vec3 m_vert[8];

    };

#endif // BOX_H
