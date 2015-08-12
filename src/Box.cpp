#include "Box.h"
#include <ngl/ShaderLib.h>
#include "ngl/VertexArrayObject.h"
#include "ngl/Vec3.h"
#include "ngl/Types.h"

const static GLubyte indices[]=  {
                                    0,1,5,0,4,5, // back
                                    3,2,6,7,6,3, // front
                                    0,1,2,3,2,0, // top
                                      4,5,6,7,6,4, // bottom
                                      0,3,4,4,7,3,
                                      1,5,2,2,6,5

                                 };
// indices for line loop

Box::Box(ngl::Vec3 _centre, ngl::Real _width, ngl::Real _height, ngl::Real _depth)
{

    m_centre = _centre;
    m_width = _width;
    m_height = _height;
    m_depth = _depth;

    m_vert[0].m_x=_centre.m_x-(_width/2.0f); m_vert[0].m_y=_centre.m_y+(_height/2.0f); m_vert[0].m_z=_centre.m_z-(_depth/2.0f);
    // + x -y -z
    m_vert[1].m_x=_centre.m_x+(_width/2.0f); m_vert[1].m_y=_centre.m_y+(_height/2.0f); m_vert[1].m_z=_centre.m_z-(_depth/2.0f);
    m_vert[2].m_x=_centre.m_x+(_width/2.0f); m_vert[2].m_y=_centre.m_y+(_height/2.0f); m_vert[2].m_z=_centre.m_z+(_depth/2.0f);
    m_vert[3].m_x=_centre.m_x-(_width/2.0f); m_vert[3].m_y=_centre.m_y+(_height/2.0f); m_vert[3].m_z=_centre.m_z+(_depth/2.0f);
    m_vert[4].m_x=_centre.m_x-(_width/2.0f); m_vert[4].m_y=_centre.m_y-(_height/2.0f); m_vert[4].m_z=_centre.m_z-(_depth/2.0f);
    m_vert[5].m_x=_centre.m_x+(_width/2.0f); m_vert[5].m_y=_centre.m_y-(_height/2.0f); m_vert[5].m_z=_centre.m_z-(_depth/2.0f);
    m_vert[6].m_x=_centre.m_x+(_width/2.0f); m_vert[6].m_y=_centre.m_y-(_height/2.0f); m_vert[6].m_z=_centre.m_z+(_depth/2.0f);
    m_vert[7].m_x=_centre.m_x-(_width/2.0f); m_vert[7].m_y=_centre.m_y-(_height/2.0f); m_vert[7].m_z=_centre.m_z+(_depth/2.0f);

    m_drawMode = GL_TRIANGLES;
    m_vao = 0;
    setVAO();
}

void Box::draw(const std::string &_shader,  ngl::Camera *_cam)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)[_shader]->use();

      ngl::Mat4 MVP= _cam->getVPMatrix() ;
    shader->setShaderParamFromMat4("MVP",MVP);

    //m_vao->draw();
}

void Box::setVAO()
{
    if( m_vao != 0 )
    {
        //std::cout<<"VAO already exists so delete and re-create\n";
        GLuint id=m_vao->getID();
        glDeleteVertexArrays(1,&id);
    }
    // if were not doing line drawing then use tris


    m_vao=ngl::VertexArrayObject::createVOA(m_drawMode);

    // now we have our data add it to the VAO, we need to tell the VAO the following
    // how much (in bytes) data we are copying
    // a pointer to the first element of data (in this case the address of the first element of the


    m_vao->bind();
    m_vao->setIndexedData(8*sizeof(ngl::Vec3),m_vert[0].m_x,sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);

    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(ngl::Vec3),0);

    m_vao->setNumIndices(sizeof(indices));
    // finally we have finished for now so time to unbind the VAO
    m_vao->unbind();

}
