#include "Ball.h"
#include "Bat.h"
#include <cmath>
#include <ngl/ShaderLib.h>
#include <stdlib.h>
#include <random>
#include <time.h>

Ball::Ball()
{
  m_pos = ngl::Vec3(0,0,0);
  m_r= 0.5f;
  m_mesh = new ngl::Obj("models/ball.obj");
  m_mesh->createVAO();
  m_v = ngl::Vec3(0,0,0);

  ngl::Colour _amb(0.9f,0.9f,0.9f);
  ngl::Colour _dif(1.0f,1.0f,1.0f);
  ngl::Colour _spec(0.50f,0.50f,0.50f);
  ngl::Real _rough(0.05f);

  m_material.setAmbient(_amb);
  m_material.setDiffuse(_dif);
  m_material.setSpecular(_spec);
  m_material.setRoughness(_rough);

  srand(time(NULL));
}

void Ball::draw(const std::string &_shader,  ngl::Camera *_cam )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();

  m_transform.setPosition(m_pos);
  ngl::Mat4 MVP= m_transform.getMatrix() * _cam->getVPMatrix() ;
  shader->setShaderParamFromMat4("MVP",MVP);

  m_mesh->draw();
}


//void Ball::batDeflect(ngl::Vec3 m_batNormal)
//{
//  //changes the velocity of the ball when colliding with the bat

//  //find the rotation required for the bat's normal to point down the z axis
//  ngl::Vec3 batNormY = (m_batNormal[0],0,m_batNormal[2]);
//  ngl::Vec3 batNormX = (0,m_batNormal[1],m_batNormal[2]);
//  ngl::Vec3 zAxis = (0,0,1);
//  float thetaY = acos(batNormY.dot(zAxis));
//  float thetaX = acos(batNormX.dot(zAxis));
//  //make this t1
//  ngl::Transformation T1;
//  //find the rotation required to allign the ball velocity vector down the YZ plane
//  ngl::Vec3 thetas = (thetaX, thetaY, 0);
//  T1.setRotation(thetas);
//  ngl::Vec3 ballVelXY = (m_v[0],m_v[1],0);
//  ngl::Real ballTheta = acos(ballVelXY.dot(zAxis));
//  //make this T2
//  ngl::Transformation T2;
//  ngl::Vec3 thetas2 = (0,0,ballTheta);
//  T2.setRotation(thetas2);
//  //make T3 the scaling through the XZ plane by -1
//  ngl::Transformation T3;
//  ngl::Vec3 reflectInXZPlane = (0,-1,0);
//  T3.setScale(reflectInXZPlane);
//  //make t4 undo t2
//  ngl::Transformation T4;
//  ngl::Vec3 T4theta = (0,0,-ballTheta);
//  T4.setRotation(T4theta);
//  //make t5 undo t1
//  ngl::Transformation T5;
//  ngl::Vec3 T5theta = (-thetaX,-thetaY,0);
//  T5.setRotation(T5theta);
//  //convert these transformations into a single matrix
//  ngl::Mat3 deflectionMatrix = T1.getMatrix()*T2.getMatrix()*T3.getMatrix()*T4.getMatrix()*T5.getMatrix();
//  ngl::Vec3 m_vNew = m_v*deflectionMatrix;

//  //return the new ball velocity
//  m_v = m_vNew;
//}

void Ball::generatePos(ngl::Real _boxWidth)
{

  float a = (-_boxWidth/2)+3;
  float b = (_boxWidth/2)-3;
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff, x;
  x = a + r;

  ngl::Real newXpos = x;
  ngl::Vec3 newPos;
  newPos.set(ngl::Vec3 (newXpos,-6.5,13));
  m_pos = newPos;
}

void Ball::generateVel()
{
  m_v = ngl::Vec3(0,0,0);
  ngl::Real x, y, z;
  float minX, maxX, minY, maxY;

  minX = -0.1f; maxX = 0.1f;
  minY = 0.45f; maxY = 0.65f;
  z=-0.7f;

  float a = minX + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxX-minX)));
  float b = minY + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxY-minY)));

  x = (ngl::Real)a; y = (ngl::Real)b;
  m_v = ngl::Vec3(x,y,z);
}



