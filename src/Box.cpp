#include "Box.h"
#include <ngl/ShaderLib.h>

Box::Box(ngl::Vec3 _centre, ngl::Real _width, ngl::Real _height, ngl::Real _depth)
{

    m_centre = _centre;
    m_width = _width;
    m_height = _height;
    m_depth = _depth;
    // create a vao as a series of GL_TRIANGLES
//    m_vao->createVOA(GL_TRIANGLES);
//    m_vao->bind();


//    const static GLubyte indices[]=  {
//        0,1,5,0,4,5, // back
//        3,2,6,7,6,3, // front
//        0,1,2,3,2,0, // top
//        4,5,6,7,6,4, // bottom
//        0,3,4,4,7,3,
//        1,5,2,2,6,5
//    };

//    GLfloat vertices[] = {_centre.m_x-_width/2,_centre.m_y+m_height/2,_centre.m_z-_depth/2,
//                          _centre.m_x+_width/2,_centre.m_y+m_height/2,_centre.m_z-_depth/2,
//                          _centre.m_x+_width/2,_centre.m_y+m_height/2,_centre.m_z+_depth/2,
//                          _centre.m_x-_width/2,_centre.m_y+m_height/2,_centre.m_z+_depth/2,
//                          _centre.m_x-_width/2,_centre.m_y-m_height/2,_centre.m_z-_depth/2,
//                          _centre.m_x+_width/2,_centre.m_y-m_height/2,_centre.m_z-_depth/2,
//                          _centre.m_x+_width/2,_centre.m_y-m_height/2,_centre.m_z+_depth/2,
//                          _centre.m_x-_width/2,_centre.m_y-m_height/2,_centre.m_z+_depth/2
//                         };

//    // in this case we are going to set our data as the vertices above

//    m_vao->setIndexedData(24*sizeof(GLfloat),vertices[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
//    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
//    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
//    m_vao->setNumIndices(sizeof(indices));
//    // now unbind
//    m_vao->unbind();

}

void Box::draw(const std::string &_shader,  ngl::Camera *_cam)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

      ngl::Mat4 MVP= _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    //m_vao->draw();
}
